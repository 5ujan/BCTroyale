
#pragma once

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<cmath>

class renderwindow
{
    private:
    SDL_Renderer *renderer;
    SDL_Texture *playertexture;
    SDL_Texture *playerflyingtexture;
    SDL_Texture *playerwtexture; 
    SDL_Texture *weapontexture ;
    SDL_Texture *weaponfiretexture ;
    SDL_Texture *healthBarRectTexture;
    SDL_Texture *healthBarTexture;
    public:
    SDL_Window *window;
    renderwindow(const char* t, int w, int h)
    {
        char* title = const_cast<char*>(t);
        
        window = SDL_CreateWindow(title,SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,w,h,SDL_WINDOW_ALLOW_HIGHDPI);
        renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED && SDL_RENDERER_PRESENTVSYNC);
        playertexture = loadTexture("assets/char.png");
        playerflyingtexture =loadTexture("assets/charnitro.png");
        playerwtexture = loadTexture("assets/charw.png");
        weapontexture = loadTexture("assets/weapon.png");
        weaponfiretexture = loadTexture("assets/weaponfire.png");
        healthBarRectTexture = loadTexture("assets/healthbarrect.png");
        healthBarTexture = loadTexture("assets/healthbar.png");
    }
    void cleanup()
    {
        SDL_DestroyWindow(window);
    }

    SDL_Texture* loadTexture(const char* filepath)
    {
        SDL_Texture *temp =NULL;
        char* fp = const_cast<char*> (filepath);
        temp = IMG_LoadTexture(renderer,fp);
        return temp;
    }
    void clear()
    {
        SDL_RenderClear(renderer);
    }
    void render(entity &ent,position camerapos, int direction =1)
    {
        SDL_Rect dst;
        dst.x=ent.getframe().x - camerapos.x;
        dst.y=ent.getframe().y - camerapos.y;
        dst.w= ent.getframe().w;
        dst.h = ent.getframe().h;
        if (direction ==1)
        {
        SDL_RenderCopy(renderer,ent.getTxt(),NULL,&dst);
        }
        else{
            SDL_RenderCopyEx(renderer,ent.getTxt(), NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
        }
    }
    void renderplayer(Player &p,position camerapos,int direction =1)
    {
        if(!p.respawning){

        SDL_Rect dst;
        SDL_Texture *t;
        dst.x=p.getframe().x - camerapos.x;
        dst.y=p.getframe().y - camerapos.y;
        dst.w= p.getframe().w;
        dst.h = p.getframe().h;
        t = playertexture;
        Uint32 currentTicks = SDL_GetTicks();
        Uint32 animationDelay = 150; // Adjust the delay to control the animation speed (milliseconds)
    
    if (p.isMovingSideways && currentTicks - p.lastAnimationUpdateTime > animationDelay)
    {
        if (t == playertexture)
        {
            t = playerwtexture;
        }
        else
        {
             
            t = playertexture;
        }
        p.lastAnimationUpdateTime = currentTicks;
    }
    if(p.isFlying || p.flying)
    {
        dst.h +=10;
        t = playerflyingtexture;
    }
    if (direction ==1)
    {
        SDL_RenderCopy(renderer,t,NULL,&dst);
    }
    else{
            SDL_RenderCopyEx(renderer,t, NULL, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
    }
    //Rendering the weapon
    SDL_Rect dst2;
    dst2.x = dst.x+15;
    dst2.y = dst.y+55;
    dst2.w = p.getweaponsize().x;
    dst2.h = p.getweaponsize().y;
    double angleDegrees = p.theta * (180.0 / M_PI);
    SDL_Texture * wt;
    if(p.firing) 
    {
        wt = weaponfiretexture;
        dst2.w+=20;
    }
    else wt = weapontexture;
    SDL_Point center = { 50 / 2, dst2.h / 2 };
    if (direction ==-1)
    SDL_RenderCopyEx(renderer, wt, NULL, &dst2, angleDegrees, &center, SDL_FLIP_VERTICAL);
    else
    SDL_RenderCopyEx(renderer, wt, NULL, &dst2, angleDegrees, &center, SDL_FLIP_NONE);
    ///rendering the name
    TTF_Font* font = TTF_OpenFont("assets/RichuMastRegular.ttf", 40);
    string textInput =p.getname();
    SDL_Color textColor = { 27, 20, 100, 100 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textInput.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_Rect dst3;
    dst3.x = dst.x - textInput.length()*10+40;
    dst3.y = dst.y - 50;
    dst3.w = textInput.length()*20;
    dst3.h = 40;

    SDL_RenderCopy(renderer, textTexture, NULL, &dst3);
    }

    }
    void render(std::vector<entity>& entities,position camerapos) {
    SDL_Rect dst;
    for (entity& e : entities) {
        dst.x = e.getframe().x - camerapos.x;
        dst.y = e.getframe().y - camerapos.y;
        dst.w = e.getframe().w;
        dst.h = e.getframe().h;
        SDL_RenderCopy(renderer, e.getTxt(),NULL, &dst);
    }

    }
    void render(std::vector<mob>& entities,position camerapos) {
    SDL_Rect dst;
    SDL_Rect Rdst;
    SDL_Rect hpdst;
    for (mob& m : entities) {
        dst.x = m.getframe().x - camerapos.x;
        dst.y = m.getframe().y - camerapos.y;
        dst.w = m.getframe().w;
        dst.h = m.getframe().h;
        Rdst.x = m.getbigrect().x-camerapos.x; 
        Rdst.y = m.getbigrect().y-camerapos.y;
        Rdst.w = m.getbigrect().w;
        Rdst.h = m.getbigrect().h;
        hpdst.x = m.getsmallrect().x-camerapos.x; 
        hpdst.y = m.getsmallrect().y-camerapos.y;
        hpdst.w = m.getsmallrect().w;
        hpdst.h = m.getsmallrect().h;
        
        SDL_RenderCopy(renderer, m.getTxt(),NULL, &dst);
        SDL_RenderCopy(renderer, m.getrecttxt(),NULL, &Rdst);
        SDL_RenderCopy(renderer, m.gethptxt(),NULL, &hpdst);
        
    }
    }
    void render(std::vector<Bullet>& bull,position cpos) {
    SDL_Rect dst;
    for (Bullet& b : bull) {
        dst.x = b.getframe().x - cpos.x;
        dst.y = b.getframe().y  - cpos.y;
        dst.w = b.getframe().w;
        dst.h = b.getframe().h;
        double angleDegrees = b.gettheta() * (180.0 / M_PI);
        SDL_Point center = { dst.w / 2, dst.h / 2 };
        SDL_RenderCopyEx(renderer, b.getTxt(), NULL, &dst, angleDegrees, &center, SDL_FLIP_NONE);
    }
    }

    void renderlives(Player &p, entity &h)
    {
        int l = p.getlives();
        SDL_Rect dst = h.getframe();

        for(int i = 0;i<l;i++)
        {
            dst.x += 50;
            SDL_RenderCopy(renderer,h.getTxt(),NULL,&dst);
        }
    }

    void rendername(string name)
    {
        TTF_Font* font = TTF_OpenFont("assets/RichuMastRegular.ttf", 40);
        string textInput =name;
        SDL_Color textColor = { 27, 20, 100, 100 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, textInput.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect dst;
        dst.x = 580- textInput.length()*10;
        dst.y = 305;
        dst.w = textInput.length()*20;
        dst.h = 40;

        SDL_RenderCopy(renderer, textTexture, NULL, &dst);
    }

    void inputname(string text)
    {
        TTF_Font* font = TTF_OpenFont("assets/Jost.ttf", 60);
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        string textInput =text;
        SDL_Color textColor = { 27, 20, 100, 150 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, textInput.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect dst;
        dst.x = 575- textInput.length()*20;
        dst.y = 380;
        dst.w = textInput.length()*40;
        dst.h = 60;

        SDL_RenderCopy(renderer, textTexture, NULL, &dst);
    }
    void rendertext(string text, position p)
    {
        TTF_Font* font = TTF_OpenFont("assets/Jost.ttf", 60);
        TTF_SetFontStyle(font, TTF_STYLE_BOLD);
        string textInput =text;
        SDL_Color textColor = { 27, 20, 100, 255 };
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, textInput.c_str(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        SDL_Rect dst;
        dst.x = p.x;
        dst.y = p.y;
        dst.w = textInput.length()*25;
        dst.h = 45;

        SDL_RenderCopy(renderer, textTexture, NULL, &dst);
    }
    void display()
    {
        SDL_RenderPresent(renderer);
    }
};
