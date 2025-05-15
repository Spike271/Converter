#ifdef _MSC_VER
    #pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
#endif

#include "popup_ui.h"

int main()
{
    auto ui = PopupWindow::create();
    ui->on_clicked([] { std::exit(0); });
    ui->run();
    return 0;
}
