#include "buffers.hpp"
#include <libidk/idk_serialize.hpp>



// uint32_t idk::storage_buffer::MAX_CAMERAS     = 1;
// uint32_t idk::storage_buffer::MAX_DIRLIGHTS   = 1;
// uint32_t idk::storage_buffer::MAX_POINTLIGHTS = 32;
// uint32_t idk::storage_buffer::MAX_SPOTLIGHTS  = 32;

// uint32_t idk::storage_buffer::MAX_TEXTURES    = 4*4096;
// uint32_t idk::storage_buffer::MAX_TRANSFORMS  = 1024;
// uint32_t idk::storage_buffer::MAX_DRAW_CALLS  = 1024;



IDK_Dirlight::IDK_Dirlight()
{
    cascades = glm::vec4(8.0f, 16.0f, 32.0f, 64.0f);
}


// struct IDK_Camera
// {
//     glm::vec4 position = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

//     glm::mat4 P = glm::mat4(1.0f);
//     glm::mat4 V = glm::mat4(1.0f);

//     glm::mat4 prev_P = glm::mat4(1.0f);
//     glm::mat4 prev_V = glm::mat4(1.0f);

//     float width, height, near=0.05f, far=512.0f;
//     float exposure=1.0f, gamma=2.2f, shutter, pad0;
//     float fov=90.0f, aspect=1.25f, bloom=0.01f, pad1;
// };



// size_t
// IDK_Camera::serialize( std::ofstream &stream ) const
// {
//     size_t n = 0;
//     n += idk::streamwrite(stream, *this);
//     return n;
// }


// size_t
// IDK_Camera::deserialize( std::ifstream &stream )
// {
//     size_t n = 0;
//     n += idk::streamread(stream, position);
//     n += idk::streamread(stream, P);
//     n += idk::streamread(stream, V);
//     n += idk::streamread(stream, width);
//     n += idk::streamread(stream, height);
//     n += idk::streamread(stream, near);
//     n += idk::streamread(stream, far);
//     n += idk::streamread(stream, exposure);
//     n += idk::streamread(stream, gamma);
//     n += idk::streamread(stream, shutter);
//     n += idk::streamread(stream, pad0);
//     n += idk::streamread(stream, fov);
//     n += idk::streamread(stream, aspect);
//     n += idk::streamread(stream, bloom);
//     n += idk::streamread(stream, pad1);
//     return n;
// }