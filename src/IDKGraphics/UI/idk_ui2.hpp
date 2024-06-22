#pragma once

#include <vector>
#include <sstream>

#include <SDL2/SDL_ttf.h>
#include <array>
#include <functional>

#include <glm/glm.hpp>
#include <libidk/GL/idk_glXXBO.hpp>
#include <libidk/GL/idk_glDrawCommand.hpp>
#include <libidk/idk_allocator.hpp>


namespace idk { class EngineAPI; class RenderEngine; }


namespace idk::ui
{
    enum ElementAlignment: uint32_t
    {
        ALIGN_LEFT   = 1<<1,
        ALIGN_RIGHT  = 1<<2,
        ALIGN_TOP    = 1<<3,
        ALIGN_BOTTOM = 1<<4,
        ALIGN_CENTER = 1<<5
    };

    struct ElementStyle
    {
        bool      invisible = false;
        glm::vec4 margin    = glm::vec4(0.0f);
        glm::vec4 padding   = glm::vec4(0.0f);
        float     radius    = 0.0f;

        glm::vec4 fg = glm::vec4(1.0f);
        glm::vec4 bg = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        glm::vec4 fg2 = glm::vec4(1.0f);
        glm::vec4 bg2 = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

        bool uniform_size = false;
    };


    struct ElementBase;
    struct Grid;
    struct List;
    struct Title;
    struct Button;
    struct Slider;
    struct TextInput;

    class LayoutManager;
    class UIRenderer;

}

namespace idkui2 = idk::ui;



struct idkui2::ElementBase
{
public:
    std::string          m_label;
    bool                 m_visible;
    ElementStyle         m_style;
    ElementStyle         m_invstyle;
    std::array<float, 4> m_bounds;
    bool                 m_focused = false;

    ElementBase( const std::string &label, const ElementStyle &style )
    :   m_label     (label),
        m_style     (style),
        m_visible   (false)
    {
        m_invstyle = m_style;

        std::swap(m_invstyle.fg,  m_invstyle.bg);
        std::swap(m_invstyle.fg2, m_invstyle.bg2);
    }

    virtual void on_down()  {  };
    virtual void on_click() {  };
    virtual void on_focus() {  };

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& ) = 0;

    void update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );

};



struct idkui2::Grid: public idkui2::ElementBase
{
    int m_rows;
    int m_cols;
    std::vector<std::vector<ElementBase*>> m_children;

    Grid( const std::string&, const ElementStyle&, int, int );

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );
    void setChild( int row, int col, ElementBase *element );

    void open()   { m_visible = true;  };
    void close()  { m_visible = false; };
    void toggle() { m_visible = !m_visible; };

};


struct idkui2::Button: public idkui2::ElementBase
{
    std::function<void()> m_callback = [](){};

    Button( const std::string &name, const ElementStyle &style, std::function<void()> callback )
    :   ElementBase(name, style)
    {
        m_callback = callback;
    }

    virtual void on_down()  {  };
    virtual void on_click() { m_callback(); };
    virtual void on_focus() {  };

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );

};


struct idkui2::Slider: public idkui2::ElementBase
{
    float &m_data;
    float  m_min, m_max, m_step;
    float  m_alpha = 0.5f;

    Slider( const std::string &name, const ElementStyle &style,
            float &data, float minvalue, float maxvalue, float step = 0.0f )
    :   ElementBase (name, style),
        m_data      (data),
        m_min       (minvalue),
        m_max       (maxvalue),
        m_step      (step)
    {

    }

    virtual void on_down();
    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );

};


struct idkui2::TextInput: public idkui2::ElementBase
{
    std::string &m_text;
    bool         m_focused = false;

    TextInput( const std::string &name, const ElementStyle &style, std::string &text )
    :   ElementBase (name, style),
        m_text      (text)
    {
        
    }

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );

};



struct idkui2::Title: public idkui2::ElementBase
{
    Title( const std::string &name, const ElementStyle &style )
    :   ElementBase(name, style)
    {

    }

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );
};



struct idkui2::List: public idkui2::ElementBase
{
    std::vector<ElementBase*> m_children_front;
    std::vector<ElementBase*> m_children_back;

    List( const std::string &name, const ElementStyle &style )
    :   ElementBase(name, style)
    {

    }

    virtual void _update( glm::vec2, glm::vec2, int, idkui2::UIRenderer& );

    void pushChildFront( ElementBase *element )
    {
        m_children_front.push_back(element);
    }

    void pushChildBack( ElementBase *element )
    {
        m_children_back.push_back(element);
    }

};



class idkui2::UIRenderer
{
private:

    static constexpr float MAX_Z_DEPTH = 128.0f;

    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texcoord;
        glm::vec3 extents;
        glm::vec4 color;
    };

    struct QuadDesc
    {
        glm::vec3 position;
        glm::vec3 extents;
        glm::vec3 fg, bg;
    };


    uint32_t                    m_VAO, m_VBO, m_IBO;
    std::vector<Vertex>         m_vertexbuffer_quad;
    std::vector<Vertex>         m_vertexbuffer_glyph;
    std::vector<uint32_t>       m_indexbuffer;

    uint32_t m_atlas;
    int      m_atlas_w;
    int      m_glyph_w;
    int      m_grid_w;

    uint32_t _renderFontAtlas( const std::string &filepath, int size );

    void     _genQuadVAO();
    void     _renderAllQuads( idk::RenderEngine&, const glm::mat4& );
    void     _renderAllText( idk::RenderEngine&, const glm::mat4& );

    int      m_nearest_button = -1;

public:

    UIRenderer( const std::string &font, int size );

    void renderQuad          ( int x, int y, int z, int w, int h, float r, const glm::vec4& );
    void renderQuadCentered  ( int x, int y, int z, int w, int h, float r, const glm::vec4& );

    void renderGlyph         ( int x, int y, int z, char c, const glm::vec4& );
    void renderGlyphCentered ( int x, int y, int z, char c, const glm::vec4& );
    void renderText          ( int x, int y, int z, const std::string&, const glm::vec4& );
    void renderTextCentered  ( int x, int y, int z, const std::string&, const glm::vec4& );
    void renderTextCenteredX ( int x, int y, int z, const std::string&, const glm::vec4& );

    void renderTexture( idk::EngineAPI &api );

};




class idkui2::LayoutManager
{
private:
    friend class ElementBase;
    friend class Grid;
    friend class Button;

    UIRenderer    m_UIRenderer;
    idkui2::Grid *m_root;

    void _updatePanel( Grid&, int depth, const glm::vec2&, const glm::vec2& );


public:

    LayoutManager( const std::string &font, int size ): m_UIRenderer(font, size) {  };

    Grid *createRootPanel( int rows, int cols, const ElementStyle& );
    void  update( idk::EngineAPI &api, float dt );
    void  renderTexture( idk::EngineAPI &api );


};


