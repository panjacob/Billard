#ifndef ___IMAGE_JK___
#define ___IMAGE_JK___


class Image {
public:
    SDL_Surface *image;
    SDL_Texture *texture;
    SDL_Rect dstrect{};

    Image(char *path, int x, int y, int w, int h, const std::shared_ptr<SDL_Renderer> &renderer) {
        image = SDL_LoadBMP(path);
        texture = SDL_CreateTextureFromSurface(renderer.get(), image);
        dstrect = {x, y, w, h};
    }


    void render(int x, int y, std::shared_ptr<SDL_Renderer> &renderer) {
        dstrect.x = x;
        dstrect.y = y;
        SDL_RenderCopy(renderer.get(), texture, nullptr, &dstrect);
    }

};

#endif