#ifndef HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006
#define HEADER_GUARD_a17e097f8bd12c4c14b2c994eb545006

struct GameState {
    virtual ~GameState() {};
    virtual void handleInput() = 0;
    virtual bool load() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void prerender() {};

    bool isPlaying = true;
};

#endif
