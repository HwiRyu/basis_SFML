#ifndef BASIS_SFML_ONE_VARIABLE_FUNCTION_H
#define BASIS_SFML_ONE_VARIABLE_FUNCTION_H

double one_variable_function(double x) {
    double C = 10;

    return C * cos(x/C);
}

void origin_function_one(sf::RenderWindow& window, sf::VertexArray& graph, double size, double one_variable_input_function(double), double start, double end) {
    graph.clear();
    for (double x = start/size; x <= end/size; x += 0.001) {
        double y = one_variable_input_function(x);
        graph.append(sf::Vertex(sf::Vector2f(x_scale(x, size), y_scale(y, size)), sf::Color::Blue));
    }
    window.draw(graph);
}


#endif //BASIS_SFML_ONE_VARIABLE_FUNCTION_H
