#pragma once
#include "../../sdk/sdk.hh"
#include "../../render/render.hh"

inline ImFont* font{};

class c_visuals {
public:
    bool draw( UWorld* world );
};

inline c_visuals* visuals;