#ifndef RENDER_PASS_H
#define RENDER_PASS_H

#include "BufferManager.h"
#include "shader.h"
#include "Camera.h"

struct RenderPass {
    Shader shader;
    std::vector<uint32_t> object_ids; // which MeshBuffers to draw

    void begin(Camera& cam, float fov, float near, float far) {
        shader.Activate();
        cam.Matrix(fov, near, far, shader, "camMatrix");
    }

    void draw(BufferManager& buffers) {
        for (uint32_t id : object_ids) {
            auto& mesh = buffers.get()[id];
            mesh.bind();
            mesh.draw();
            mesh.unbind();
        }
    }
};

#endif