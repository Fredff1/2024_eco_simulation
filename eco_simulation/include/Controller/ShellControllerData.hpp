#ifndef SHELL_CONTROLLER_H
#define SHELL_CONTROLLER_H

enum shellCmdEvent{
    NO_SHELL_EVENT,
    PAUSE_MAIN_MODEL,
    CONTINUE_MAIN_MODEL,
    QUIT_MAIN_MODEL,
};

struct shellControllerData{
    shellCmdEvent event;

    shellControllerData(){
        event=NO_SHELL_EVENT;
    }
};

#endif /* C4728992_0859_4E23_BFC4_FD34A8B3E39C */
