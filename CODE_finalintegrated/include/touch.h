#ifndef TOUCH_H
#define TOUCH_H

class Touch {
    public:
        Touch(void);
        ~Touch(void);

        int get_X_position(void);
        int get_Y_position(void);
        void touch_init(void);
};

#endif