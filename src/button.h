#ifndef button_h
#define button_h

class button
{
private:
    SDL_Rect button_rect;
    SDL_Color button_color = {255,255,255};
public:
    button(){};
    ~button(){};
    button(int x, int y, int w, int h)
    {
        button_rect = {x,y,w,h};
    }

    void set_x(int x)
    {
        button_rect.x = x;
    }

    SDL_Rect rect(){return button_rect;}
    bool hover()
    {
        int x,y;
        SDL_GetMouseState(&x,&y);

        if(x < button_rect.x)                    return false;
        if(x > button_rect.x + button_rect.w)    return false;
        if(y < button_rect.y)                    return false;
        if(y > button_rect.y + button_rect.h)    return false;
        return true;
    }

    void set_color(SDL_Color button_color)
    {
        this->button_color = button_color;
    }

    void render(SDL_Renderer* &renderer)
    {
        SDL_SetRenderDrawColor(renderer,button_color.r,button_color.g,button_color.b,255);
        SDL_RenderFillRect(renderer,&button_rect);
        SDL_RenderDrawRect(renderer,&button_rect);
        SDL_SetRenderDrawColor(renderer,255,255,255,255);
    }
};
#endif // button_h

