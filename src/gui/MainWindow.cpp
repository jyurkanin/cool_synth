#include "MainWindow.hpp"

void MainWindow::render()
{

    using namespace ::juce::gl;

    if(!m_is_initialized) return;
        
    jassert(OpenGLHelpers::isContextActive());

    float desktop_scale = openGLContext.getRenderingScale();
        
    OpenGLHelpers::clear(juce::Colours::black);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        const ScopedLock lock(m_mutex);
        glViewport(0, 0,
                   roundToInt(desktop_scale * bounds.getWidth()),
                   roundToInt(desktop_scale * bounds.getHeight()));
    }

    m_shader->use();

    m_uniforms->projection_matrix->setMatrix4(getProjectionMatrix().mat, 1, false);
    m_uniforms->view_matrix->setMatrix4(getViewMatrix().mat, 1, false);

    m_shape->draw(*m_attributes);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER);
}

void MainWindow::paint(Graphcs& g)
{
    g.setColour(juce::Colours::green);
    g.setFont(20);
    g.drawText("OpenGL Example", 25, 20 , 300, 30, Justification::left);
    g.drawLine(20, 20, 170, 20);
    g.drawLine(20, 50, 170, 50);
}

void MainWindow::createShaders()
{
    constexpr char *vertex_shader =
        "attribute vec4 position;\n"
        "attribute vec4 source_colour;\n"
        "attribute vec2 texture_coord_in;\n"
        "\n"
        "uniform mat4 projection_matrix;\n"
        "uniform mat4 view_matrix;\n"
        "\n"
        "varying vec4 destination_colour;\n"
        "varying vec2 texture_coord_out;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    destination_colour = source_colour;\n"
        "    texture_coord_out = texture_coord_in;\n"
        "    gl_Position = projection_matrix * view_matrix * position;\n"
        "}\n";

    constexpr char *fragment_shader =
        "varying vec4 destination_colour;\n"
        "varying vec2 texture_coord_out;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec4 colour = vec4(0.95, 0.57, 0.03, 0.7);\n"
        "    gl_FragColor = colour;\n"
        "}\n";

    std::unique_ptr<OpenGLShaderProgram> new_shader(new OpenGLShaderProgram(openGLContext));

    String status_text;

    if(new_shader->addVertexShader(OpenGLHelpers::translateVertexShaderToV3(vertex_shader))
       && new_shader->addFragmentShader(OpenGLHelpers::translateFragmentShaderToV3(fragment_shader))
       && new_shader->link())
    {
        m_shape.reset();
        m_uniforms.reset();
        m_attributes.reset();

        m_shader = std::move(new_shader);
        m_shader->use();

        m_shape.reset(new Shape());
        m_attributes.reset(new Attibutes(*m_shader));
        m_uniforms.reset(new Uniforms(*m_shader));
    }
}
