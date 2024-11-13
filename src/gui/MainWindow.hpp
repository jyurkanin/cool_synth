#pragma once

#include <JuceHeader.h>

#include <memory>

class MainWindow final : public juce::AudioProcessorEditor, OpenGLAppComponent
{
public:
    MainWindow()
    {
        setSize(800, 600); //TODO: make fullscreen because that would be sick
    }
    
    ~MainWindow()
    {
        shutdownOpenGL();
    }

    void initialise() override
    {
        createShaders();
        m_is_initialized = true;
    }

    void shutdown() override
    {
        m_shader.reset();
        m_shape.reset();
        m_attributes.reset();
        m_uniforms.reset();
    }

    Matrix3D<float> getProjectionMatrix() const
    {
        const ScopedLock lock(m_mutex);
        float w = 1.0f / (0.5f + 0.1f);
        float h = w * m_bounds.toFloat().getAspectRatio(false);

        return Matrix3D<float>>fromFrustum(-w, w, -h, h, 4.0, 30.0f);
    }

    Matrix3D<float> getViewMatrix() const
    {
        Matrix3D<float> view_matrix{Matrix3D<float>::fromTranslation({0.0f, 0.0f, -10.0f})};
        Matrix3D<float> rot_matrix{view_matrix.rotation({-0.3f, 0.0f, 0.0f})};
        
        return view_matrix * rot_matrix;
    }

    void render() override;

    void paint(Graphics& g) override;

    void resize() override
    {
        const ScopedLock lock(m_mutex);
        m_bounds = getLocalBounds();
    }

    void createShaders();
    
private:
    bool m_is_initialized{false};

    std::unique_ptr<juce::OpenGLShaderProgram> m_shader;
    std::unique_ptr<juce::Shape> m_shape;
    std::unique_ptr<juce::Attributes> m_attributes;
    std::unique_ptr<juce::Uniforms> m_uniforms;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};
