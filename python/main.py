from OpenGL.GL import *
from OpenGL.GLU import *
import ctypes
import glm

import numpy as np
import pygame

# i'm too lazy to do open()
from pathlib import Path

def get_file_contents(path_str):
    return Path(path_str).read_text()

def create_program(vertex_shader_contents, fragment_shader_contents):
    vertex_shader_id = glCreateShader(GL_VERTEX_SHADER)
    glShaderSource(vertex_shader_id, vertex_shader_contents)
    glCompileShader(vertex_shader_id)

    vertex_status = None
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, vertex_status)

    if vertex_status == GL_FALSE:
        print("vertex shader failed to compile: " + glGetShaderInfoLog(vertex_shader_id))

    fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER)
    glShaderSource(fragment_shader_id, fragment_shader_contents)
    glCompileShader(fragment_shader_id)

    fragment_status = None
    glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, fragment_status)

    if fragment_status == GL_FALSE:
        print("fragment shader failed to compile: " + glGetShaderInfoLog(fragment_shader_id))

    program_id = glCreateProgram()
    glAttachShader(program_id, vertex_shader_id)
    glAttachShader(program_id, fragment_shader_id)
    glLinkProgram(program_id)

    link_status = None
    glGetProgramiv(program_id, GL_LINK_STATUS, link_status)

    if link_status == GL_FALSE:
        print("program failed to link two shaders: " + glGetProgramInfoLog(program_id))

    glDeleteShader(vertex_shader_id)
    glDeleteShader(fragment_shader_id)
    
    return program_id

def main():
    pygame.init()

    surface = pygame.display.set_mode((600, 600), pygame.DOUBLEBUF | pygame.OPENGL)

    clock = pygame.time.Clock()

    # setup cube stuff

    cube_vertex = glGenVertexArrays(1)
    glBindVertexArray(cube_vertex)

    cube_buffer = glGenBuffers(1)
    glBindBuffer(GL_ARRAY_BUFFER, cube_buffer)

    cube_vertices = np.array([
		-1.0, 1.0, 1.0,
		1.0, -1.0, 1.0,
		-1.0, -1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,
		1.0, 1.0, -1.0,

		1.0, 1.0, -1.0, 
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		-1.0, 1.0, -1.0,

		-1.0, 1.0, -1.0,
		-1.0, -1.0, 1.0,
		-1.0, -1.0, -1.0,
		-1.0, 1.0, 1.0,

		-1.0, -1.0, -1.0,
		1.0, -1.0, 1.0,
		-1.0, -1.0, 1.0,
		1.0, -1.0, -1.0,

		-1.0, 1.0, -1.0,
		1.0, 1.0, 1.0,
		-1.0, 1.0, 1.0,
		1.0, 1.0, -1.0,
    ], dtype=np.float32)

    glBufferData(GL_ARRAY_BUFFER, cube_vertices, GL_STATIC_DRAW)

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, None)
    glEnableVertexAttribArray(0)

    cube_elements = glGenBuffers(1)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_elements)

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, np.array([
            0, 1, 2,
            0, 1, 3,

            4, 5, 6,
            4, 5, 7,

            8, 9, 10,
            8, 9, 11,

            12, 13, 14,
            12, 13, 15,

            16, 17, 18,
            16, 17, 19,

            20, 21, 22,
            20, 21, 23,
        ], dtype=np.uint32), GL_STATIC_DRAW)


    cube_program = create_program('''\
#version 330 core

layout (location = 0) in vec3 pos;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(pos, 1.0f);
}
    ''', '''\
#version 330 core

void main()
{
    vec2 coord = vec2(gl_FragCoord.x / 800.0f, gl_FragCoord.y / 600.0f);
    gl_FragColor = vec4(1.0f - sin(coord.y * 1), sin(coord.x * 2), 0.0f, 1.0f);
}
    ''')

    gluPerspective(50, surface.get_width()/surface.get_height(), 0.1, 100.0)
    glEnable(GL_DEPTH_TEST)

    while True:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                exit(0)

        dt = clock.tick(60) * .001

        # c++ remnants!
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.1, 0.1, 0.1, 1);

        glUseProgram(cube_program)
        
        transform_matrix = glm.mat4(1.0)
        transform_matrix = glm.scale(transform_matrix, glm.vec3(0.5, 0.5, 0.5))
        transform_matrix = glm.rotate(transform_matrix, pygame.time.get_ticks() * .001, glm.vec3(0, 1, 1))

        glUniformMatrix4fv(glGetUniformLocation(cube_program, "transform"), 1, GL_FALSE, glm.value_ptr(transform_matrix))

        glBindVertexArray(cube_vertex)
        glDrawElements(GL_TRIANGLES, 39, GL_UNSIGNED_INT, None)
        #  glDrawArrays(GL_TRIANGLES, 0, 3)
        
        pygame.display.flip()
    

if __name__ == '__main__':
    main()
