#include <iostream>
#include "neuralnet.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>

#define WIDTH 600
#define HEIGHT 600
#define RADIUS 10.f
#define BATCHSIZE 100
uint8_t pixels[WIDTH * HEIGHT * 4];
NeuralNetwork net;

int main() {
    char color = 'R';
    bool training = false;
    float aux[2];
    // Criando a janela
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "TEST");
    //window.setFramerateLimit(60);
    // Criando o vetor com os inputs para a rede neural
    std::vector<sf::Vector2i> inputs;
    // Criando o vetor com os outputs esperados para a rede neural
    std::vector<sf::Vector3f> outputs; 
    // Vetor com as coordenadas do mouse
    sf::Vector2i mouse_position;
    // Entidades para desenho
    sf::Texture texture;
    sf::Sprite sprite;
    sf::CircleShape circle(RADIUS);
    texture.create(WIDTH, HEIGHT);
    // -----------------------------------------------------------------------------------
    sf::Color col1(128, 61, 59, 255);
    sf::Color col2(175, 130, 96, 255);
    sf::Color col3(50, 44, 43, 255);

    float net_inputs[BATCHSIZE][input_size];
    float net_outputs[BATCHSIZE][output_size];
    bool mouse_was_pressed = false;
    for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
        pixels[i] = 0;
        pixels[i + 1] = 0;
        pixels[i + 2] = 0;
        pixels[i + 3] = 255;
    }
    texture.update(pixels);
    sprite.setTexture(texture);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

        }
        sf::Vector3f color1(1.0f, 0.0f, 0.0f);
        sf::Vector3f color2(0.0f, 1.0f, 0.0f);
        sf::Vector3f color3(0.0f, 0.0f, 1.0f);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if (!mouse_was_pressed) {
                mouse_position = sf::Mouse::getPosition(window);
                inputs.push_back(mouse_position);
                if (color == 'R')
                    outputs.push_back(color1);
                else if (color == 'B')
                    outputs.push_back(color2);
                else
                    outputs.push_back(color3);
                mouse_was_pressed = true;
            }
        } else {
            mouse_was_pressed = false;
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            color = 'R';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
            color = 'B';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
            color = 'G';
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            training = true;

        if (!training) {
            for (int i = 0; i < inputs.size(); i++) {
                net_inputs[i][0] = (float)inputs[i].x / WIDTH;
                net_inputs[i][1] = (float)inputs[i].y / HEIGHT;
                net_outputs[i][0] = outputs[i].x;
                net_outputs[i][1] = outputs[i].y;
                net_outputs[i][2] = outputs[i].z;
            }
        }
        else {
            for(int i = 0 ; i < 100 ; i++)
                net.Train_Batch(net_inputs, net_outputs, (int)inputs.size()); 
            for (int i = 0; i < WIDTH * HEIGHT * 4; i += 4) {
                aux[0] = ((i / 4) % WIDTH) / (float)WIDTH; 
                aux[1] = (i / (4 * WIDTH)) / (float)HEIGHT; 
                net.Forward_Pass(aux);
                if (net.index_max_output == 0) {
                    pixels[i] = col1.r;
                    pixels[i + 1] = col1.g;
                    pixels[i + 2] = col1.b;
                }
                else if (net.index_max_output == 1){
                    pixels[i] = col2.r;
                    pixels[i + 1] = col2.g;
                    pixels[i + 2] = col2.b;
                }
                else {
                    pixels[i] = col3.r;
                    pixels[i + 1] = col3.g;
                    pixels[i + 2] = col3.b;
                }
            }
        }
        texture.update(pixels); 
        sprite.setTexture(texture); 
        window.clear();
        window.draw(sprite);
        for (int i = 0; i < inputs.size(); i++) {
            circle.setPosition((float)inputs[i].x - RADIUS, (float)inputs[i].y - RADIUS);
            if (outputs[i] == color1)
                circle.setFillColor(col1);
            else if(outputs[i] == color2)
                circle.setFillColor(col2);
            else
                circle.setFillColor(col3);
            window.draw(circle);
        }
        window.display();
    }

	return 0;
}