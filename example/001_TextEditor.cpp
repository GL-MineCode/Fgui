// 001_TextEditor.cpp — 基于 Fgui 的文本编辑器示例
//
// 功能：
//   - 多标签页编辑（TabControl，每标签一个 InputBox，支持中文 IME、选区、Ctrl+C/X/V/A）
//   - 撤销/重做（基于 InputBox 新增的 OnChange 回调做状态快照，每标签独立）
//   - 状态栏（文件名/字符数/行数/修改标志）+ 窗口标题/标签页标题 * 指示
//   - 关闭未保存标签、退出前弹「保存/不保存/取消」确认框
//   - 快捷键：Ctrl+N/O/S/W、Ctrl+Shift+S、Ctrl+Z/Y、Tab 插入制表符
//

//一点点Trick:事实上由于懒癌，我还没有做MenuBar，所以菜单栏就用多个ComboBox模拟，你肯定没想到ComboBox无选项默认文本还能这么用

//非常明显，这个示例绝大部分逻辑是D指导写的，问就是我懒
//由于目前还没有控件排版引擎，所以自动排版布局是手动实现的（还好有D指导协助 :D）

#define NOMINMAX

#include <cstdio>
#include <deque>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

#include "decode.hpp"
#define DefaultColorKit ColorKitPresets::Ocean
#include "Fgui\Fgui.hpp"
#include "SDLWindowStuff.hpp"
#include "SDL_EventPlus.hpp"
#include "GL_Commdlg.hpp"

// 行号栏控件
class LineNumberGutter : public Fgui_Control{
public:
    InputBox* editor = nullptr;   // 关联的编辑控件
    FontEx*   font = nullptr;
    int       ptsize = 22;
private:
    int last_cam_y = -1;
    int last_view_h = -1;
public:
    LineNumberGutter(InputBox* _editor, FontEx* _font, int _ptsize, const SDL_Rect& rect = {0,0,0,0}){
        editor = _editor;
        font = _font;
        ptsize = _ptsize;
        default_rect = rect;
        this->InvalidateRect();
    }

    int GetLineCount() const{
        if(!editor) return 1;
        std::string buf = editor->GetBuffer("\n");
        int n = 1;
        for(char c : buf) if(c == '\n') n++;
        return n;
    }

    // 行高：优先按编辑器内容平均行高（与编辑区渲染精确对齐）
    int GetLineHeight() const{
        if(editor){
            int total_h = editor->GetContentHeight();
            int lines = GetLineCount();
            if(lines > 0 && total_h > 0) return std::max(1, total_h / lines);
        }
        return font ? std::max(font->GetPrimaryFontHeight(ptsize), 8) : 20;
    }

    void ActionOnPaint(SDL_Renderer* renderer, const SDL_Rect&, const SDL_Rect& relative_rect) override{
        SDL_Rect draw_area = default_rect;
        draw_area.x = relative_rect.x;
        draw_area.y = relative_rect.y;

        // 背景（略深于编辑区，与滚动条同色系）
        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BackgroundColorDarker));
        SDL_RenderFillRect(renderer, &draw_area);

        if(font && editor){
            int cam_y = editor->GetScrollY();
            int lh = GetLineHeight();
            int total = GetLineCount();
            if(lh > 0 && total > 0){
                int first = std::max(0, cam_y / lh);
                int last  = std::min(total - 1, (cam_y + draw_area.h) / lh);
                SDL_Color num_c = color_kit.ForegroundColor;
                for(int i = first; i <= last; i++){
                    int y = i * lh - cam_y + lh / 2;
                    std::string num = std::to_string(i + 1);
                    font->paintText_Blended(renderer, draw_area.x + draw_area.w / 2,
                                            draw_area.y + y, ptsize, num, num_c, {0.5f, 0.5f});
                }
            }
        }

        // 右侧分隔线
        SDL_SetRenderDrawColor(renderer, ColorArg(color_kit.BorderColor));
        SDL_RenderDrawLine(renderer, draw_area.x + draw_area.w - 1, draw_area.y,
                           draw_area.x + draw_area.w - 1, draw_area.y + draw_area.h);
    }

    void ActionOnEvent(const SDL_Event*, const SDL_Rect&) override{
        // 行号栏不交互
    }

    void ActionOnTimer(const SDL_Rect&) override{
        // 跟随编辑器纵向滚动 / 视口高度变化而重绘
        int cam_y = editor ? editor->GetScrollY() : 0;
        int view_h = default_rect.h;
        if(cam_y != last_cam_y || view_h != last_view_h){
            last_cam_y = cam_y;
            last_view_h = view_h;
            this->InvalidateRect();
        }
    }

    std::string ActionOnGetTypeName() const override{
        return "Example.LineNumberGutter";
    }
};

// 单个文档（标签页）的状态
struct DocState{
    std::string tab_id;                       // 对应 TabControl 标签页 ID
    std::string path;                         // 文件路径（空=未命名）
    bool modified = false;                    // 是否有未保存修改
    std::string saved_buffer;                 // 最后一次保存/加载的内容
    std::string newline = "\r\n";             // 换行风格（打开时检测）
    std::deque<std::string> undo_stack;       // 撤销栈
    std::deque<std::string> redo_stack;       // 重做栈
    std::string last_buffer;                  // OnChange 前的缓冲区快照
    std::shared_ptr<ControlBox> page;         // 标签页内容容器（TabControl 自动缩放）
    std::shared_ptr<InputBox>  editor;        // 编辑控件
    std::shared_ptr<LineNumberGutter> gutter; // 行号栏（独立控件，跟随 cam_y）
};

// ==================== 编辑器全局状态 ====================
static std::shared_ptr<TabControl>        g_tabs;         // 多标签容器
static std::map<std::string, DocState>    g_docs;         // 标签 ID -> 文档状态
static int  g_next_tab_id = 1;
static std::string g_last_active_id;                      // 上一帧激活的标签 ID
static constexpr size_t MAX_UNDO = 100;

static std::shared_ptr<StaticText>        g_status;       // 状态栏
static std::shared_ptr<ControlBox>        g_welcome;      // 欢迎页容器
static std::shared_ptr<StaticText>        g_welcome_title, g_welcome_hint;
static std::shared_ptr<PushButton>        g_welcome_new, g_welcome_open;
static std::vector<std::shared_ptr<ComboBox>> g_menus;    // 菜单（ComboBox 模拟）
static std::shared_ptr<ControlBox>        g_cb;           // 根容器
static std::vector<int>                   g_menu_natural_w; // 菜单自然宽度

static SDLWindowStuff* g_sws = nullptr;                   // 窗口（标题栏用）
static FontEx*         g_font = nullptr;

static bool g_quit_requested = false;                     // 菜单「退出」请求

// 布局常量
static const int g_menu_h   = 44;                         // 菜单栏高度
static const int g_status_h = 30;                         // 状态栏高度
static const int g_gutter_w = 58;                         // 行号栏宽度（编辑区左移为其让位）

// 前置声明
static DocState* ActiveDoc();
static bool ConfirmCloseDoc(DocState& d);
static void UpdateStatusText();
static void UpdateWindowTitle();

static size_t CountUtf8Chars(const std::string& s){
    size_t n = 0;
    for(unsigned char c : s){
        if((c & 0xC0) != 0x80) n++;
    }
    return n;
}

static std::string Basename(const std::string& path){
    size_t slash = path.find_last_of("/\\");
    return (slash == std::string::npos) ? path : path.substr(slash + 1);
}


static bool FileExists(const std::string& path){
    std::error_code ec;
    return std::filesystem::exists(
        std::filesystem::path(reinterpret_cast<const char8_t*>(path.c_str())), ec);
}

// 解析字体路径：exe 位于 example/ 时字体在上级目录；从根目录运行时用相对名
static std::vector<std::string> ResolveFontPaths(){
    std::vector<std::string> paths;
    for(const char* p : {"../font.ttf", "font.ttf"}){
        if(FileExists(p)) paths.push_back(p);
    }
    if(paths.empty()) paths.push_back("font.ttf"); // 让 FontEx 抛出清晰的错误
    return paths;
}

// 当前激活文档（无标签页或找不到时返回 nullptr）
static DocState* ActiveDoc(){
    if(!g_tabs) return nullptr;
    TabControl::TabItem* tab = g_tabs->GetActiveTab();
    if(!tab) return nullptr;
    auto it = g_docs.find(tab->id);
    return (it != g_docs.end()) ? &it->second : nullptr;
}

static void UpdateWindowTitle(){
    if(!g_sws) return;
    DocState* d = ActiveDoc();
    std::string title;
    if(d){
        std::string name = d->path.empty() ? "未命名" : Basename(d->path);
        title = std::string(d->modified ? "* " : "") + name + " — Fgui 文本编辑器";
    }else{
        title = "Fgui 文本编辑器";   // 欢迎页（无标签）
    }
    SDL_SetWindowTitle(g_sws->native_handle, title.c_str());
}

static void UpdateStatusText(){
    if(!g_status) return;
    DocState* d = ActiveDoc();
    if(!d){ g_status->SetText("无文档"); return; }
    std::string content = d->editor->GetBuffer("\n");
    size_t chars = CountUtf8Chars(content);
    int lines = 1;
    for(char c : content) if(c == '\n') lines++;

    char buf[512];
    snprintf(buf, sizeof(buf), "%s | 字符: %zu | 行: %d | %s",
        d->path.empty() ? "未命名" : Basename(d->path).c_str(),
        chars, lines, d->modified ? "已修改" : "未修改");
    g_status->SetText(buf);
}

// 更新某文档对应的标签页标题（未保存加 * 前缀）
static void UpdateTabTitle(DocState& d){
    if(!g_tabs) return;
    try{
        TabControl::TabItem& tab = g_tabs->GetTab(d.tab_id);
        std::string base = d.path.empty() ? "未命名" : Basename(d.path);
        tab.name = std::string(d.modified ? "* " : "") + base;
    }catch(...){}
    g_tabs->InvalidateRect();
}

static void SetDocModified(DocState& d, bool m){
    if(d.modified != m){
        d.modified = m;
        UpdateTabTitle(d);
        UpdateStatusText();
        UpdateWindowTitle();
    }
}

// ==================== 文件 I/O ====================

static bool ReadTextFile(const std::string& path, std::string& out){
    // 路径为 UTF-8，转宽字符后使用宽字符 API，兼容中文路径
    std::wstring wpath = Utf8ToUtf16(path);
    FILE* f = _wfopen(wpath.c_str(), L"rb");
    if(!f) return false;
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    if(len < 0){ fclose(f); out.clear(); return true; }   // 空文件
    out.resize((size_t)len);
    bool ok = true;
    if(len > 0 && fread(&out[0], 1, (size_t)len, f) != (size_t)len) ok = false;
    fclose(f);
    return ok;
}

static bool WriteTextFile(const std::string& path, const std::string& content){
    // 路径为 UTF-8，转宽字符后使用宽字符 API，兼容中文路径
    std::wstring wpath = Utf8ToUtf16(path);
    FILE* f = _wfopen(wpath.c_str(), L"wb");
    if(!f) return false;
    bool ok = true;
    if(!content.empty() && fwrite(content.data(), 1, content.size(), f) != content.size()) ok = false;
    fclose(f);
    return ok;
}

// 检测首个换行的风格（\r\n 或 \n）
static void DetectNewlineStyle(const std::string& raw, std::string& out){
    size_t p = raw.find('\n');
    if(p == std::string::npos){ out = "\r\n"; return; }
    out = (p > 0 && raw[p - 1] == '\r') ? "\r\n" : "\n";
}

// ==================== 缓冲区/撤销管理 ====================

// 程序化设置全文（SetBuffer 不触发 OnChange，这里手动同步；新文档清空撤销/重做栈）
static void SetDocContent(DocState& d, const std::string& content){
    d.editor->SetBuffer(content);
    d.last_buffer = d.editor->GetBuffer("\n");
    d.undo_stack.clear();
    d.redo_stack.clear();
}

// 新增一个标签页，返回其文档指针
static DocState* AddTab(const std::string& path, const std::string& content, const std::string& newline){
    std::string id = "doc" + std::to_string(g_next_tab_id++);
    DocState& d = g_docs[id];
    d.tab_id = id;
    d.path = path;
    d.newline = newline;

    d.page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0}, -1, -1);
    d.editor = d.page->PushbackControl("editor",
        std::make_shared<InputBox>("", g_font, 22,
            FGUI_INPUTBOX_FLAG_ALLOW_MULTILINES |
            FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL | FGUI_INPUTBOX_FLAG_HORIZONTAL_SCROLL,   // 内置垂直滚动条（自动跟随光标）
            SDL_Rect{g_gutter_w, 0, 0, 0}));
    // 行号栏：独立的左侧控件，跟随编辑器的 cam_y 滚动
    d.gutter = d.page->PushbackControl("gutter",
        std::make_shared<LineNumberGutter>(d.editor.get(), g_font, 22, SDL_Rect{0, 0, g_gutter_w, 0}));

    // 文本变更：推撤销快照 + 更新修改标志 + 标签标题 + 刷新行号栏
    d.editor->actions.OnChange = [&d](InputBox* box){
        std::string cur = box->GetBuffer("\n");
        if(cur == d.last_buffer) return;         // 防御重复
        d.undo_stack.push_back(d.last_buffer);   // 变更前的快照
        if(d.undo_stack.size() > MAX_UNDO) d.undo_stack.pop_front();
        d.redo_stack.clear();                    // 新编辑清空重做栈
        d.last_buffer = cur;
        SetDocModified(d, cur != d.saved_buffer);
        if(d.gutter) d.gutter->InvalidateRect(); // 行数可能变化
    };

    SetDocContent(d, content);
    d.saved_buffer = d.last_buffer;   // 新标签视为已保存

    std::string base = path.empty() ? "未命名" : Basename(path);
    g_tabs->PushbackTab(TabControl::TabItem{base, id, d.page});
    return &d;
}

// 关闭某个标签（已确认未保存处理），不触发 OnTabClose
static void CloseDoc(DocState& d){
    std::string id = d.tab_id;
    g_tabs->EraseTab(id);
    g_docs.erase(id);
}

// 关闭激活标签页（含未保存确认）
static void CloseActiveTab(){
    DocState* d = ActiveDoc();
    if(!d) return;
    if(!ConfirmCloseDoc(*d)) return;
    CloseDoc(*d);
}

static void Undo(){
    DocState* d = ActiveDoc();
    if(!d || d->undo_stack.empty()) return;
    d->redo_stack.push_back(d->last_buffer);
    if(d->redo_stack.size() > MAX_UNDO) d->redo_stack.pop_front();
    d->last_buffer = d->undo_stack.back();
    d->undo_stack.pop_back();
    d->editor->SetBuffer(d->last_buffer);
    d->editor->InvalidateRect();
    if(d->gutter) d->gutter->InvalidateRect();
    SetDocModified(*d, d->last_buffer != d->saved_buffer);
    UpdateStatusText();
}

static void Redo(){
    DocState* d = ActiveDoc();
    if(!d || d->redo_stack.empty()) return;
    d->undo_stack.push_back(d->last_buffer);
    if(d->undo_stack.size() > MAX_UNDO) d->undo_stack.pop_front();
    d->last_buffer = d->redo_stack.back();
    d->redo_stack.pop_back();
    d->editor->SetBuffer(d->last_buffer);
    d->editor->InvalidateRect();
    if(d->gutter) d->gutter->InvalidateRect();
    SetDocModified(*d, d->last_buffer != d->saved_buffer);
    UpdateStatusText();
}

// ==================== 文件操作 ====================

static bool SaveDocTo(DocState& d, const std::string& path){
    std::string content = d.editor->GetBuffer(d.newline);
    if(!WriteTextFile(path, content)){
        GLDLG::messageBox("保存失败", "无法写入文件：\n" + path, {{1, "确定"}});
        return false;
    }
    d.path = path;
    d.saved_buffer = d.editor->GetBuffer("\n");
    SetDocModified(d, false);
    return true;
}

static bool SaveDocAs(DocState& d){
    std::string default_name = d.path.empty() ? "未命名.txt" : Basename(d.path);
    std::string path = GLDLG::getSaveFileName(
        {"文本文件 (*.txt)|*.txt", "所有文件 (*.*)|*.*"},
        "另存为", "", default_name, "txt");
    if(path.empty()) return false;
    return SaveDocTo(d, path);
}

static bool SaveDoc(DocState& d){
    if(d.path.empty()) return SaveDocAs(d);
    return SaveDocTo(d, d.path);
}

static bool SaveActiveFile(){
    DocState* d = ActiveDoc();
    return d ? SaveDoc(*d) : false;
}
static bool SaveActiveFileAs(){
    DocState* d = ActiveDoc();
    return d ? SaveDocAs(*d) : false;
}

// 关闭某文档前的未保存确认；返回 true=可关闭
static bool ConfirmCloseDoc(DocState& d){
    if(!d.modified) return true;
    int choice = GLDLG::messageBox("未保存的修改",
        std::string("文档 \"") + (d.path.empty() ? "未命名" : Basename(d.path)) + "\" 有未保存的修改。\n\n是否保存？",
        {{1, "保存"}, {2, "不保存"}, {3, "取消"}});
    if(choice == 3) return false;
    if(choice == 1) return SaveDoc(d);
    return true;
}

// 退出前确认：任何文档有未保存修改则询问
static bool ConfirmExit(){
    bool any = false;
    for(auto& kv : g_docs) if(kv.second.modified){ any = true; break; }
    if(!any) return true;
    int choice = GLDLG::messageBox("未保存的修改",
        "存在未保存的修改。\n\n退出前是否保存？",
        {{1, "保存"}, {2, "不保存"}, {3, "取消"}});
    if(choice == 3) return false;
    if(choice == 1){
        for(auto& kv : g_docs){
            if(kv.second.modified){
                if(!SaveDoc(kv.second)) return false;   // 任一另存为被取消则中止退出
            }
        }
    }
    return true;
}

// 新建：新增一个空白标签页（不清空当前页）
static void NewFile(){
    if(!g_tabs) return;
    DocState* d = AddTab("", "", "\r\n");
    if(d){
        g_tabs->ActivateTab(d->tab_id);
        UpdateStatusText();
        UpdateWindowTitle();
    }
}

// 打开：在新建标签页中打开文件
static void OpenFile(){
    std::string path = GLDLG::getOpenFileName(
        {"文本文件 (*.txt)|*.txt", "所有文件 (*.*)|*.*"},
        "打开文件");
    if(path.empty()) return;
    std::string raw;
    if(!ReadTextFile(path, raw)){
        GLDLG::messageBox("打开失败", "无法读取文件：\n" + path, {{1, "确定"}});
        return;
    }
    std::string nl;
    DetectNewlineStyle(raw, nl);
    DocState* d = AddTab(path, raw, nl);
    if(d){
        g_tabs->ActivateTab(d->tab_id);
        UpdateStatusText();
        UpdateWindowTitle();
    }
}

// ==================== 编辑操作（供菜单调用） ====================

static void MenuCopy(){
    DocState* d = ActiveDoc();
    if(!d) return;
    std::string sel = d->editor->GetSelection();
    if(!sel.empty()) SDL_SetClipboardText(sel.c_str());
}

static void MenuCut(){
    DocState* d = ActiveDoc();
    if(!d) return;
    std::string sel = d->editor->GetSelection();
    if(!sel.empty()){
        SDL_SetClipboardText(sel.c_str());
        d->editor->DeleteText();   // 触发 OnChange
    }
}

static void MenuPaste(){
    DocState* d = ActiveDoc();
    if(!d) return;
    char* txt = SDL_GetClipboardText();
    if(txt && txt[0] != '\0'){
        d->editor->InsertText(txt); // 触发 OnChange
    }
    if(txt) SDL_free(txt);
}

static void MenuSelectAll(){
    DocState* d = ActiveDoc();
    if(d) d->editor->SelectAll();
}

// 所有菜单恢复显示为菜单标题
static void ResetAllMenus(){
    for(auto& m : g_menus) m->SelectItem((int)0xFFFFFFFF);
}

static void HandleMenuCommand(int id){
    switch(id){
        case 1:  NewFile(); break;                       // 新建
        case 2:  OpenFile(); break;                      // 打开...
        case 3:  SaveActiveFile(); break;                // 保存
        case 4:  SaveActiveFileAs(); break;              // 另存为...
        case 5:  g_quit_requested = true; break;         // 退出
        case 6:  CloseActiveTab(); break;                // 关闭标签页
        case 10: Undo(); break;                          // 撤销
        case 11: Redo(); break;                          // 重做
        case 12: MenuCut(); break;                       // 剪切
        case 13: MenuCopy(); break;                      // 复制
        case 14: MenuPaste(); break;                     // 粘贴
        case 15: MenuSelectAll(); break;                 // 全选
        case 20:
            GLDLG::messageBox("关于",
                "Fgui 文本编辑器示例\n\n"
                "基于 Fgui + GL_Commdlg",
                {{1, "确定"}});
            break;
        default: break;
    }
    ResetAllMenus();
}

// 构造一个菜单（ComboBox 模拟）
static std::shared_ptr<ComboBox> MakeMenu(const std::string& title,
                                          const std::vector<ComboBox::ComboItem>& items){
    auto combo = std::make_shared<ComboBox>(title, items, (int)0xFFFFFFFF, g_font, 24, SDL_Rect{0, 0, 0, 0});
    // ComboBox 默认按 1.5 倍文本宽自适应，作为菜单栏按钮略窄，这里加宽到至少 96px
    if(combo->default_rect.w < 96) combo->default_rect.w = 96;
    combo->actions.OnChange = [](ComboBox*, const ComboBox::ComboItem& item){
        HandleMenuCommand(item.id);
    };
    return combo;
}

// 菜单栏：高度铺满顶栏（g_menu_h），宽度保持自适应（合适即可），横排留 8px 间距
static void LayoutMenuBar(){
    int x = 0;
    for(size_t i = 0; i < g_menus.size(); i++){
        int w = g_menu_natural_w[i];
        if(w < 96) w = 96;
        g_menus[i]->default_rect = SDL_Rect{x, 0, w, g_menu_h};
        g_menus[i]->InvalidateRect();
        x += w + 8;
    }
}

// 每帧把各标签页行号栏与 InputBox 尺寸同步到其内容容器（TabControl 会缩放激活页内容容器）
static void SyncTabEditorSizes(){
    for(auto& kv : g_docs){
        DocState& d = kv.second;
        if(d.editor && d.page){
            int pw = d.page->default_rect.w;
            int ph = d.page->default_rect.h;
            if(pw > 0 && ph > 0){
                int ew = pw - g_gutter_w;
                if(ew < 20) ew = 20;
                if(d.gutter &&
                   (d.gutter->default_rect.w != g_gutter_w || d.gutter->default_rect.h != ph)){
                    d.gutter->default_rect = SDL_Rect{0, 0, g_gutter_w, ph};
                    d.gutter->InvalidateRect();
                }
                if(d.editor->default_rect.x != g_gutter_w || d.editor->default_rect.y != 0 ||
                   d.editor->default_rect.w != ew || d.editor->default_rect.h != ph){
                    d.editor->default_rect = SDL_Rect{g_gutter_w, 0, ew, ph};
                    d.editor->InvalidateRect();
                }
            }
        }
    }
}

// 欢迎页内容水平居中
static void CenterWelcome(int win_w){
    if(g_welcome_title) g_welcome_title->default_rect.x = (win_w - g_welcome_title->default_rect.w) / 2;
    if(g_welcome_hint)   g_welcome_hint->default_rect.x  = (win_w - g_welcome_hint->default_rect.w) / 2;
    if(g_welcome_new)    g_welcome_new->default_rect.x   = (win_w - g_welcome_new->default_rect.w) / 2;
    if(g_welcome_open)   g_welcome_open->default_rect.x  = (win_w - g_welcome_open->default_rect.w) / 2;
    if(g_welcome) g_welcome->InvalidateRect();
}

// 根据是否有标签页切换「欢迎页 / 标签区」的可见性（无标签页时显示欢迎页）
static void UpdateWelcomeVisibility(){
    if(!g_cb) return;
    bool has_tabs = g_tabs && g_tabs->GetTabCount() > 0;
    bool changed = false;
    if(g_welcome){
        bool wv = g_welcome->IsVisibility();
        if(wv != !has_tabs){ g_welcome->SetVisibility(!has_tabs); changed = true; }
    }
    if(g_tabs){
        bool tv = g_tabs->IsVisibility();
        if(tv != has_tabs){ g_tabs->SetVisibility(has_tabs); changed = true; }
    }
    if(changed) g_cb->InvalidateRect();
}

// 根据窗口尺寸重排菜单栏、标签区与状态栏
static void LayoutControls(int win_w, int win_h){
    LayoutMenuBar();
    CenterWelcome(win_w);

    int cont_w = win_w;
    int cont_h = win_h - g_menu_h - g_status_h;
    if(cont_h < 20) cont_h = 20;
    if(g_tabs){
        g_tabs->default_rect = SDL_Rect{0, g_menu_h, cont_w, cont_h};
        g_tabs->InvalidateRect();
    }
    if(g_welcome){
        g_welcome->default_rect = SDL_Rect{0, g_menu_h, cont_w, cont_h};
        g_welcome->InvalidateRect();
    }
    if(g_status){
        g_status->default_rect = SDL_Rect{8, win_h - g_status_h, 0, 0};
        g_status->InvalidateRect();
    }
    UpdateStatusText();
    // 状态栏文本高度可能使矩形底部超出窗口（导致根容器误判内容溢出而显示滚动条），贴底钳制
    if(g_status && g_status->default_rect.y + g_status->default_rect.h > win_h){
        g_status->default_rect.y = win_h - g_status->default_rect.h;
        g_status->InvalidateRect();
    }
}

int main(int argc, char** argv){
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "1");

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();

    g_sws = SDLWindowStuff::Create("Fgui 文本编辑器", 1000, 800,
        SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN);
    g_sws->SetDarkMode(true);
    g_sws->SetPreference(3);
    SDL_Renderer* renderer = g_sws->CreateRenderer();
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Event eve;
    EventPlus evp(&eve, g_sws->native_handle);
    Timer fps_lim(60);
    bool mainloop = true;

    // 字体：exe 位于 example/ 目录，字体在上级目录；也兼容从根目录运行
    FontEx font(ResolveFontPaths());
    g_font = &font;

    Fgui_ConfigControlAnimation(true);

    int win_w = 1000, win_h = 800;
    SDL_GetWindowSize(g_sws->native_handle, &win_w, &win_h);
    SDL_Rect top_relative_rect = {0, 0, win_w, win_h};

    g_cb = std::make_shared<ControlBox>(SDL_Rect{0, 0, win_w, win_h}, -1, -1);
    g_cb->SetShowScrollBars(false);   // 根容器为窗口级外壳，不显示滚动条（内容始终贴齐窗口）

    // ---- 菜单栏（ComboBox 模拟，高度铺满顶栏、宽度自适应）----
    {
        auto menu_file = MakeMenu("文件", {
            {"新建(N)", 1, 0}, {"打开(O)...", 2, 0},
            {"保存(S)", 3, 0}, {"另存为(A)...", 4, 0},
            {"关闭标签页(W)", 6, 0}, {"退出(X)", 5, 0},
        });
        g_cb->PushbackControl("menu_file", menu_file);
        g_menus.push_back(menu_file);
        g_menu_natural_w.push_back(menu_file->default_rect.w);

        auto menu_edit = MakeMenu("编辑", {
            {"撤销(U)", 10, 0}, {"重做(R)", 11, 0},
            {"剪切(T)", 12, 0}, {"复制(C)", 13, 0},
            {"粘贴(P)", 14, 0}, {"全选(A)", 15, 0},
        });
        g_cb->PushbackControl("menu_edit", menu_edit);
        g_menus.push_back(menu_edit);
        g_menu_natural_w.push_back(menu_edit->default_rect.w);

        auto menu_help = MakeMenu("帮助", {
            {"关于", 20, 0},
        });
        g_cb->PushbackControl("menu_help", menu_help);
        g_menus.push_back(menu_help);
        g_menu_natural_w.push_back(menu_help->default_rect.w);

        LayoutMenuBar();
    }

    // ---- 多标签页编辑区 ----
    int cont_h0 = win_h - g_menu_h - g_status_h;
    if(cont_h0 < 20) cont_h0 = 20;
    g_tabs = g_cb->PushbackControl("tabs",
        std::make_shared<TabControl>(
            TAB_FLAG_CLOSE_BUTTON | TAB_FLAG_ALLOW_REARRANGE | TAB_FLAG_POS_TOP,
            &font, 18, SDL_Rect{0, g_menu_h, win_w, cont_h0}));

    // 关闭标签页：先确认未保存修改，允许后移除文档状态
    g_tabs->actions.OnTabClose = [](TabControl::TabItem& tab){
        auto it = g_docs.find(tab.id);
        if(it == g_docs.end()) return true;
        if(!ConfirmCloseDoc(it->second)) return false;
        g_docs.erase(it);      // 关闭后移除文档状态（内容由 TabControl 移除）
        return true;
    };
    g_tabs->actions.OnTabActive = [](TabControl::TabItem&){
        return true;   // 状态栏/标题在帧末按激活标签刷新
    };
    g_tabs->actions.OnTabRearrange = [](TabControl::TabItem&){};

    // ---- 欢迎页（无标签页时显示；启动即欢迎页，而非新建文件）----
    g_welcome = g_cb->PushbackControl("welcome",
        std::make_shared<ControlBox>(SDL_Rect{0, g_menu_h, win_w, cont_h0}, -1, -1));

    g_welcome_title = g_welcome->PushbackControl("welcome.title",
        std::make_shared<StaticText>("欢迎使用 Fgui 文本编辑器", &font, 32, SDL_Rect{0, 80, 0, 0}));
    g_welcome_hint = g_welcome->PushbackControl("welcome.hint",
        std::make_shared<StaticText>("新建或打开一个文件开始编辑", &font, 20, SDL_Rect{0, 130, 0, 0}));

    g_welcome_new = g_welcome->PushbackControl("welcome.new",
        std::make_shared<PushButton>("新建文件", &font, 24, SDL_Rect{0, 190, 0, 0}));
    g_welcome_new->actions.OnClick = [](PushButton*){ NewFile(); };

    g_welcome_open = g_welcome->PushbackControl("welcome.open",
        std::make_shared<PushButton>("打开文件...", &font, 24, SDL_Rect{0, 250, 0, 0}));
    g_welcome_open->actions.OnClick = [](PushButton*){ OpenFile(); };

    CenterWelcome(win_w);
    UpdateWelcomeVisibility();   // 初始无标签 → 显示欢迎页

    // ---- 状态栏 ----
    g_status = g_cb->PushbackControl("status",
        std::make_shared<StaticText>("", &font, 18, SDL_Rect{8, win_h - g_status_h + 4, 0, 0}));

    LayoutControls(win_w, win_h);   // 铺满菜单栏、定位标签区与状态栏
    UpdateWindowTitle();

    g_sws->Show();

    while(mainloop){
        evp.reset();
        while(evp.poll()){
            if(eve.window.windowID == SDL_GetWindowID(g_sws->native_handle)){
                // ---- 快捷键（在转发给控件之前拦截）----
                if(eve.type == SDL_KEYDOWN){
                    Uint16 mod = eve.key.keysym.mod;
                    bool ctrl = (mod & KMOD_CTRL) != 0;
                    SDL_Scancode sc = eve.key.keysym.scancode;
                    if(ctrl && (mod & KMOD_SHIFT) && sc == SDL_SCANCODE_Z){ Redo(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_Z){ Undo(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_Y){ Redo(); continue; }
                    if(ctrl && (mod & KMOD_SHIFT) && sc == SDL_SCANCODE_S){ SaveActiveFileAs(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_S){ SaveActiveFile(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_W){ CloseActiveTab(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_N){ NewFile(); continue; }
                    if(ctrl && sc == SDL_SCANCODE_O){ OpenFile(); continue; }
                    if(!ctrl && sc == SDL_SCANCODE_TAB){
                        DocState* d = ActiveDoc();
                        if(d) d->editor->InsertText("\t");
                        continue;
                    }
                }

                if(eve.window.windowID == SDL_GetWindowID(g_sws->native_handle)) g_cb->MaintainEvent(&eve, top_relative_rect);

                if(eve.type == SDL_WINDOWEVENT){
                    if(eve.window.event == SDL_WINDOWEVENT_CLOSE){
                        if(ConfirmExit()) mainloop = false;
                    }
                    else if(eve.window.event == SDL_WINDOWEVENT_RESIZED){
                        SDL_GetWindowSize(g_sws->native_handle, &win_w, &win_h);
                        top_relative_rect.w = win_w;
                        top_relative_rect.h = win_h;
                        g_cb->default_rect.w = win_w;
                        g_cb->default_rect.h = win_h;
                        LayoutControls(win_w, win_h);
                        g_cb->InvalidateRect();
                    }
                    else if(eve.window.event == SDL_WINDOWEVENT_EXPOSED){
                        g_cb->InvalidateRect();
                    }
                }
            }
        }

        // 菜单「退出」请求（弹确认框）
        if(g_quit_requested){
            g_quit_requested = false;
            if(ConfirmExit()) mainloop = false;
        }

        g_cb->MaintainTimer(top_relative_rect);
        SyncTabEditorSizes();   // InputBox 跟随内容容器尺寸
        UpdateWelcomeVisibility();   // 无标签页时显示欢迎页

        // 激活标签变化时刷新状态栏/标题，并聚焦新激活页编辑区
        if(g_tabs){
            TabControl::TabItem* at = g_tabs->GetActiveTab();
            std::string id = at ? at->id : "";
            if(id != g_last_active_id){
                auto old = g_docs.find(g_last_active_id);
                if(old != g_docs.end()) old->second.editor->SetFocus(false);
                g_last_active_id = id;
                UpdateStatusText();
                UpdateWindowTitle();
                if(DocState* nd = ActiveDoc()){
                    nd->editor->SetFocus(true);
                    SDL_StartTextInput();
                }
            }
        }

        SDL_Rect dirty_rect = g_cb->GetInvaildRect(top_relative_rect);
        if(dirty_rect.w != 0 && dirty_rect.h != 0){
            SDL_SetRenderDrawColor(renderer,
                g_cb->GetColorKit().BackgroundColorDarker.r,
                g_cb->GetColorKit().BackgroundColorDarker.g,
                g_cb->GetColorKit().BackgroundColorDarker.b, 255);
            SDL_RenderFillRect(renderer, &dirty_rect);
            g_cb->MaintainRender(renderer, top_relative_rect);
            SDL_RenderPresent(renderer);
        }

        fps_lim.Delay();
    }

    SDL_DestroyRenderer(renderer);
    g_sws->Release();
    TTF_Quit();
    SDL_Quit();
    return 0;
}