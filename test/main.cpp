#define NOMINMAX

#include <cstdio>
#include <SDL_image.h>
#include "decode.hpp"
#include "SDLWindowStuff.hpp"
#include "SDL_EventPlus.hpp"
#include "Times.hpp"
#include "DebugText.hpp"
#include "StopWatch.hpp"
#include "SmoothCpp.hpp"
#define ENABLE_QRCODE_MODULE
#include "Fgui\Fgui.hpp"

int main(int,char**){
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
    SDL_SetHint(SDL_HINT_IME_SHOW_UI,"1");
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY,"0");
    SDLWindowStuff* sws = SDLWindowStuff::Create("程序模板",1000,800,SDL_WINDOW_ALLOW_HIGHDPI);
    sws->SetDarkMode(true);
    sws->SetPreference(3);
    SDL_Renderer* renderer = sws->CreateRenderer();
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);

    SDL_Event eve;
    EventPlus evp(&eve);
    Timer fps_lim(60);
    bool mainloop = true;

    bool enable_heatmap = true;

    //Fgui_ConfigControlAnimation(true);

    FontEx font({
        "font.ttf"
    });

    //BUG: 宽度拓展不工作
    std::shared_ptr<ControlBox> cb = std::make_shared<ControlBox>(SDL_Rect{0,0,1000,800},2000,9000);

    //cb->PushbackControl("scroll",std::make_shared<ScrollBarV>(1000,500,SDL_Rect{980,0,20,800}));

    cb->PushbackControl("enable_control_anim",std::make_shared<StaticText>("启用控件动画",&font,23,SDL_Rect{10,20}));
    {
        auto ctl = std::make_shared<ToggleButton>(false,SDL_Rect{150,10,100,50});
        ctl->actions.OnChange = [](ToggleButton* tb,bool state){
            if(state){
                Fgui_ConfigControlAnimation(true);
            }
            else{
                Fgui_ConfigControlAnimation(false);
            }
        };
        cb->PushbackControl("toggle_anim",ctl);
    }

    cb->PushbackControl("enable_heatmap",std::make_shared<StaticText>("显示热力图",&font,23,SDL_Rect{260,20}));
    {
        auto ctl = std::make_shared<ToggleButton>(true,SDL_Rect{390,10,100,50});
        ctl->actions.OnChange = [&enable_heatmap,&cb](ToggleButton* tb,bool state){
            enable_heatmap = state;
            cb->InvalidateRect();
        };
        cb->PushbackControl("toggle_heatmap",ctl);
    }

    {
        auto ctl = std::make_shared<ComboBox>("-",
            std::vector<ComboBox::ComboItem>{
                ComboBox::ComboItem{"默认主题",1,100},
                ComboBox::ComboItem{"深色主题",2,100},
                ComboBox::ComboItem{"浅色主题",3,100},
                ComboBox::ComboItem{"蓝色主题",4,100},
                ComboBox::ComboItem{"绿色主题",5,100},
                ComboBox::ComboItem{"红色主题",6,100},
                ComboBox::ComboItem{"橙色主题",7,100},
                ComboBox::ComboItem{"紫色主题",8,100},
                ComboBox::ComboItem{"终端风格",9,100},
                ComboBox::ComboItem{"海洋主题",10,100},
                ComboBox::ComboItem{"日落主题",11,100}
            },1,&font,30,SDL_Rect{510,10,0,0});
        ctl->actions.OnChange = [cb](ComboBox*,const ComboBox::ComboItem& item){
            Fgui_ColorKit ck = ColorKitPresets::Default;
            switch(item.id){
                case 1:  ck = ColorKitPresets::Default;   break;
                case 2:  ck = ColorKitPresets::Dark;      break;
                case 3:  ck = ColorKitPresets::Light;     break;
                case 4:  ck = ColorKitPresets::Blue;      break;
                case 5:  ck = ColorKitPresets::Green;     break;
                case 6:  ck = ColorKitPresets::Red;       break;
                case 7:  ck = ColorKitPresets::Orange;    break;
                case 8:  ck = ColorKitPresets::Purple;    break;
                case 9:  ck = ColorKitPresets::Terminal;  break;
                case 10: ck = ColorKitPresets::Ocean;     break;
                case 11: ck = ColorKitPresets::Sunset;    break;
            }
            cb->ActionOnColorKitChange(ck);
        };
        cb->PushfrontControl("theme_selector",ctl);
    }

    {
        auto ctl = std::make_shared<PushButton>("按钮",&font,46,SDL_Rect{200,100});
        ctl->actions.OnClick = [](PushButton* button){
            printf("Test\n");
            if(button->GetText() == "按钮"){
                button->SetText("切换状态");
            }
            else{
                button->SetText("按钮");
            }
        };
        cb->PushbackControl("button1",ctl);
    }

    cb->PushbackControl("text1",std::make_shared<StaticText>("文本",&font,46,SDL_Rect{100,100}));

    {
        auto ctl = std::make_shared<LinkText>("http://www.bilibili.com",&font,46,SDL_Rect{100,200});
        ctl->actions.OnClick = [](LinkText* button){
            ShellExecuteW(NULL, L"open", L"http://www.bilibili.com", NULL, NULL, SW_SHOWNORMAL);
        };
        cb->PushbackControl("link1",ctl);
    }

    auto input1 = cb->PushbackControl("input1",std::make_shared<InputBox>("单击输入...",&font,46,0,SDL_Rect{100,300,500,0}));

    {
        auto ctl = std::make_shared<CheckBox>(SDL_Rect{100,400,50,50});
        ctl->actions.OnChange = [](CheckBox* cb,CheckBox::CheckBoxState state){
            if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_CHECKED){
                printf("cb1:Checked!\n");
            }
            else if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_UNCHECKED){
                printf("cb1:Unchecked!\n");
            }
        };
        cb->PushbackControl("checkbox1",ctl);
    }
    {
        auto ctl = std::make_shared<CheckBox>(SDL_Rect{160,400,50,50});
        ctl->actions.OnChange = [](CheckBox* cb,CheckBox::CheckBoxState state){
            if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_CHECKED){
                printf("cb2:Checked!\n");
            }
            else if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_UNCHECKED){
                printf("cb2:Unchecked!\n");
            }
        };
        cb->PushbackControl("checkbox2",ctl);
    }
    {
        auto ctl = std::make_shared<CheckBox>(SDL_Rect{220,400,50,50});
        ctl->actions.OnChange = [](CheckBox* cb,CheckBox::CheckBoxState state){
            if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_CHECKED){
                printf("cb3:Checked!\n");
            }
            else if(state == CheckBox::CheckBoxState::CHECKBOX_STATE_UNCHECKED){
                printf("cb3:Unchecked!\n");
            }
        };
        cb->PushbackControl("checkbox3",ctl);
    }

    cb->FindControl<CheckBox>("checkbox1")->SetState(CheckBox::CheckBoxState::CHECKBOX_STATE_INDETERMINATE);
    cb->FindControl<CheckBox>("checkbox2")->SetState(CheckBox::CheckBoxState::CHECKBOX_STATE_INDETERMINATE);
    cb->FindControl<CheckBox>("checkbox3")->SetState(CheckBox::CheckBoxState::CHECKBOX_STATE_INDETERMINATE);

    {
        auto ctl = std::make_shared<RadioButton>(SDL_Rect{100,500,40,40});
        ctl->actions.OnChange = [](RadioButton* cb){
            printf("rb1:Selected!\n");
        };
        cb->PushbackControl("radiobutton1",ctl);
    }
    {
        auto ctl = std::make_shared<RadioButton>(SDL_Rect{150,500,40,40});
        ctl->actions.OnChange = [](RadioButton* cb){
            printf("rb2:Selected!\n");
        };
        cb->PushbackControl("radiobutton2",ctl);
    }
    {
        auto ctl = std::make_shared<RadioButton>(SDL_Rect{200,500,40,40});
        ctl->actions.OnChange = [](RadioButton* cb){
            printf("rb3:Selected!\n");
        };
        cb->PushbackControl("radiobutton3",ctl);
    }
    {
        auto ctl = std::make_shared<RadioButton>(SDL_Rect{250,500,40,40});
        ctl->actions.OnChange = [](RadioButton* cb){
            printf("rb4:Selected!\n");
        };
        cb->PushbackControl("radiobutton4",ctl);
    }

    {
        auto ctl = std::make_shared<ToggleButton>(false,SDL_Rect{100,550,100,50});
        ctl->actions.OnChange = [](ToggleButton* tb,bool state){
            if(state){
                printf("On!\n");
            }
            else{
                printf("Off!\n");
            }
        };
        cb->PushbackControl("togglebutton1",ctl);
    }

    SDL_Surface* sur = IMG_Load("gunmu.png");

    cb->PushbackControl("picturebox1",std::make_shared<PictureBox>(SDL_Rect{620,100,200,200},sur));

    SDL_FreeSurface(sur);

    auto subcb = cb->PushbackControl("controlbox1",std::make_shared<ControlBox>(SDL_Rect{500,400,400,400},500,800));

    subcb->PushbackControl("controlbox1.text1", std::make_shared<StaticText>("文本", &font, 46, SDL_Rect{100, 100}));
    {
        auto ctl = std::make_shared<PushButton>("按钮", &font, 46, SDL_Rect{200, 100});
        ctl->actions.OnClick = [](PushButton *button){
            printf("Test\n");
            if(button->GetText() == "按钮"){
                button->SetText("切换状态");
            }
            else{
                button->SetText("按钮");
            }
        };
        subcb->PushbackControl("controlbox1.button1", ctl);
    }

    // 嵌套 ControlBox 内加入可滚动 InputBox：验证滚轮应只滚动该 InputBox（及其所在 subcb），而不牵连根容器
    subcb->PushbackControl("controlbox1.input", std::make_shared<InputBox>(
        "嵌套输入框：\n第一行\n第二行\n第三行\n第四行\n第五行\n第六行\n第七行\n第八行\n第九行\n第十行",
        &font, 20, FGUI_INPUTBOX_FLAG_ALLOW_MULTILINES | FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL, SDL_Rect{300, 220, 180, 150}));

    {
        auto ctl = std::make_shared<Slider>(0.0f,100.0f,50.0f,SDL_Rect{100,600,300,50});
        ctl->actions.OnBeginDragging = [](Slider*,float v){
            printf("开始拖动:%f\n",v);
        };
        ctl->actions.OnDragging = [](Slider*,float v){
            printf("正在拖动:%f\n",v);
        };
        ctl->actions.OnEndDragging = [](Slider*,float v){
            printf("结束拖动:%f\n",v);
        };
        cb->PushfrontControl("slider",ctl);
    }

    auto qrcode = cb->PushfrontControl("qrcode",std::make_shared<QRCodeBox>("Hello,world! 喜欢你！",qrcodegen::QrCode::Ecc::MEDIUM,4,1,SDL_Rect{100,650,300,300}));

    {
        auto ctl = std::make_shared<ComboBox>("--请选择--",
            std::vector<ComboBox::ComboItem>{
                ComboBox::ComboItem{"选项A",1,100},
                ComboBox::ComboItem{"选项B",2,100},
                ComboBox::ComboItem{"选项C",3,100}
            },(int)0xFFFFFFFF,&font,46,SDL_Rect{420,820,0,0});
        ctl->actions.OnChange = [](ComboBox*,const ComboBox::ComboItem& item){
            printf("选择了:%s(%d)\n",item.ui_string.c_str(),item.id);
        };
        cb->PushfrontControl("combobox",ctl);
    }

    auto input2 = cb->PushbackControl("input2", std::make_shared<InputBox>("单击输入多行文本...", &font, 23,FGUI_INPUTBOX_FLAG_ALLOW_MULTILINES | FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL | FGUI_INPUTBOX_FLAG_HORIZONTAL_SCROLL,SDL_Rect{100, 960, 500, 500}));

    auto input3 = cb->PushbackControl("input3", std::make_shared<InputBox>("", &font, 23,FGUI_INPUTBOX_FLAG_PASSWORD,SDL_Rect{100, 1470, 500, 0}));

    auto input4 = cb->PushbackControl("input4", std::make_shared<InputBox>(
        "噫吁嚱，危乎高哉！\n"
        "蜀道之难，难于上青天！\n"
        "蚕丛及鱼凫，开国何茫然！\n"
        "尔来四万八千岁，不与秦塞通人烟。\n"
        "西当太白有鸟道，可以横绝峨眉巅。\n"
        "地崩山摧壮士死，然后天梯石栈相钩连。\n"
        "上有六龙回日之高标，下有冲波逆折之回川。\n"
        "黄鹤之飞尚不得过，猿猱欲度愁攀援。\n"
        "青泥何盘盘，百步九折萦岩峦。\n"
        "扪参历井仰胁息，以手抚膺坐长叹。"
        , &font, 23,FGUI_INPUTBOX_FLAG_READ_ONLY | FGUI_INPUTBOX_FLAG_VERTICAL_SCROLL | FGUI_INPUTBOX_FLAG_HORIZONTAL_SCROLL,SDL_Rect{100, 1540, 500, 300}));

    auto pb_normal = std::make_shared<ProgressBar>(0.0f, 100.0f, 0.0f, &font, 23, ProgressBar::PROGRESSBAR_STATE_NORMAL, SDL_Rect{100, 1900, 500, 40});
    cb->PushbackControl("pb_normal", pb_normal);

    {
        auto ctl = std::make_shared<ProgressBar>(0.0f,100.0f,0.0f,&font,23,ProgressBar::PROGRESSBAR_STATE_PROCESSING,SDL_Rect{100,1950,500,40});
        cb->PushbackControl("pb_processing",ctl);
    }
    
    // 示例1：顶部条 + 关闭按钮 + 允许拖动重排
    {
        auto tab_ctl = std::make_shared<TabControl>(TAB_FLAG_CLOSE_BUTTON | TAB_FLAG_ALLOW_REARRANGE, &font, 23, SDL_Rect{100, 2050, 900, 450});
        tab_ctl->actions.OnTabClose = [](TabControl::TabItem& tab){
            printf("关闭标签页:%s\n", tab.name.c_str());
            return true;
        };
        tab_ctl->actions.OnTabActive = [](TabControl::TabItem& tab){
            printf("激活标签页:%s\n", tab.name.c_str());
            return true;
        };
        tab_ctl->actions.OnTabRearrange = [](TabControl::TabItem& tab){
            printf("重排标签页:%s\n", tab.name.c_str());
        };

        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("tab1.text", std::make_shared<StaticText>("这是标签页一", &font, 23, SDL_Rect{20,20}));
            auto tab1_btn = std::make_shared<PushButton>("按钮一", &font, 23, SDL_Rect{20,60});
            tab1_btn->actions.OnClick = [](PushButton* b){ printf("标签页一按钮被点击！\n"); };
            page->PushbackControl("tab1.btn", tab1_btn);
            tab_ctl->PushbackTab(TabControl::TabItem{"标签一","tab1",page});
        }
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("tab2.text", std::make_shared<StaticText>("这是标签页二", &font, 23, SDL_Rect{20,20}));
            page->PushbackControl("tab2.input", std::make_shared<InputBox>("输入框二", &font, 23, 0, SDL_Rect{20,60,300,0}));
            tab_ctl->PushbackTab(TabControl::TabItem{"标签二","tab2",page});
        }
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("tab3.text", std::make_shared<StaticText>("这是标签页三", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"标签三","tab3",page});
        }
        tab_ctl->ActivateTab("tab1");
        cb->PushbackControl("tabcontrol1", tab_ctl);
    }

    // 示例2：顶部条 + 禁止拖动重排（对比验证 TAB_FLAG_ALLOW_REARRANGE）
    {
        auto tab_ctl = std::make_shared<TabControl>(TAB_FLAG_NONE, &font, 23, SDL_Rect{100, 2530, 900, 350});
        tab_ctl->actions.OnTabActive = [](TabControl::TabItem& tab){
            printf("(无重排)激活标签页:%s\n", tab.name.c_str());
            return true;
        };
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("nr1", std::make_shared<StaticText>("不可重排 - 页A", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"页A","a",page});
        }
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("nr2", std::make_shared<StaticText>("不可重排 - 页B", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"页B","b",page});
        }
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("nr3", std::make_shared<StaticText>("不可重排 - 页C", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"页C","c",page});
        }
        tab_ctl->ActivateTab("a");
        cb->PushbackControl("tabcontrol2", tab_ctl);
    }

    // 示例3：左侧条
    {
        auto tab_ctl = std::make_shared<TabControl>(TAB_FLAG_POS_LEFT | TAB_FLAG_ALLOW_REARRANGE, &font, 23, SDL_Rect{100, 2910, 700, 300});
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("left1", std::make_shared<StaticText>("左侧标签页 1", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"左一","l1",page});
        }
        {
            auto page = std::make_shared<ControlBox>(SDL_Rect{0,0,0,0});
            page->PushbackControl("left2", std::make_shared<StaticText>("左侧标签页 2", &font, 23, SDL_Rect{20,20}));
            tab_ctl->PushbackTab(TabControl::TabItem{"左二","l2",page});
        }
        tab_ctl->ActivateTab("l1");
        cb->PushbackControl("tabcontrol_left", tab_ctl);
    }

    const SDL_Rect top_relative_rect = {0,0,1000,800};

    SDL_Texture* buf = SDL_CreateTexture(renderer,SDL_PIXELFORMAT_RGBA8888,SDL_TEXTUREACCESS_TARGET,top_relative_rect.w,top_relative_rect.h);

    struct HotArea{
        SDL_Rect rect;
        Uint64 timestamp;
    };

    std::vector<HotArea> ha;


    int fps_count = 0;

    while(mainloop){
        evp.reset();
		while (evp.poll())
		{
            if(eve.window.windowID == SDL_GetWindowID(sws->native_handle)){
                cb->MaintainEvent(&eve,top_relative_rect);
                if(eve.type == SDL_WINDOWEVENT){
                    if(eve.window.event == SDL_WINDOWEVENT_CLOSE){
                        mainloop = false;
                    }
                }
                else if(eve.type == SDL_WINDOWEVENT){
                    if(eve.window.event == SDL_WINDOWEVENT_EXPOSED){
                        cb->InvalidateRect();
                    }
                }
            }
		}

        if((fps_count % 15) == 0){
            pb_normal->SetValue(float(fps_count % 150) / 1.5f,true);
        }

        if(input1->GetBuffer() != qrcode->GetContent()){
            qrcode->SetContent(input1->GetBuffer());
        }
        cb->MaintainTimer(top_relative_rect);
        SDL_Rect dirty_rect = cb->GetInvaildRect(top_relative_rect);
        if(dirty_rect.w != 0 && dirty_rect.h != 0){
            //printf("{%d,%d,%d,%d} Updated!\n",dirty_rect.x,dirty_rect.y,dirty_rect.w,dirty_rect.h);

            //auto i = cb->FindControl<ControlBox>("controlbox1");

            //printf("{%d,%d,%d,%d} TEST!\n",i->GetRelativeInvaildRect().x,i->GetRelativeInvaildRect().y,i->GetRelativeInvaildRect().w,i->GetRelativeInvaildRect().h);

            std::vector<HotArea>::iterator it = ha.begin();
            for(;it != ha.end();it++){
                if(SDL_RectEquals(&it->rect,&dirty_rect)){
                    it->timestamp = SDL_GetTicks64();
                    break;
                }
            }
            if(it == ha.end()) ha.push_back({dirty_rect,SDL_GetTicks64()});

            if(enable_heatmap){
                SDL_SetRenderTarget(renderer,buf);
                SDL_SetRenderDrawColor(renderer,cb->GetColorKit().BackgroundColorDarker.r,cb->GetColorKit().BackgroundColorDarker.g,cb->GetColorKit().BackgroundColorDarker.b,255);      
                SDL_RenderFillRect(renderer,&dirty_rect);
                cb->MaintainRender(renderer,top_relative_rect);
                SDL_SetRenderTarget(renderer,NULL);
            }
            else{
                SDL_SetRenderDrawColor(renderer,cb->GetColorKit().BackgroundColorDarker.r,cb->GetColorKit().BackgroundColorDarker.g,cb->GetColorKit().BackgroundColorDarker.b,255);     
                SDL_RenderFillRect(renderer,&dirty_rect);
                cb->MaintainRender(renderer,top_relative_rect);
                SDL_RenderPresent(renderer);
            }
        }

        if(enable_heatmap){
            SDL_RenderCopy(renderer,buf,NULL,NULL);
            for(std::vector<HotArea>::iterator i = ha.begin();i < ha.end();){
                Uint64 span = SDL_GetTicks64() - i->timestamp;
                if(span > 1000){
                    i = ha.erase(i);
                    continue;
                }
                int a = 255 - 255.0f * (float(span) / 1000.0f);
                SDL_SetRenderDrawColor(renderer,255,20,20,a);   
                SDL_RenderDrawRect(renderer,&i->rect);
                paintDBTextFormat(renderer,i->rect.x,i->rect.y,{255,20,20,(uint8_t)a},2,{0,0},"%llu",span);
                i++;
            }
            SDL_RenderPresent(renderer);
        }

        fps_lim.Delay();
        fps_count++;
    }

    SDL_DestroyTexture(buf);

    SDL_DestroyRenderer(renderer);
    sws->Release();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return 0;
}
