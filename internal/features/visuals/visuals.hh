#pragma once
#include "../../sdk/sdk.hh"
#include "../../render/render.hh"

inline ImFont* font{};

class c_visuals {
public:
    bool draw( UWorld* world );
private:
    const wchar_t* GOB( AActor* Actor );
};

inline c_visuals* visuals;