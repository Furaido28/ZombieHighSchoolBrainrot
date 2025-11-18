#ifndef MENU_H
#define MENU_H

    #include <SFML/Graphics.hpp>
    #include <vector>

    class Menu {
    public:
        Menu(float width, float height);

        void draw(sf::RenderWindow &window);
        void moveUp();
        void moveDown();
        int getSelectedIndex() const;

        bool isMouseOnItem(sf::Vector2f mousePos, int index) const;
        void hoverWithMouse(sf::Vector2f mousePos); // (optionnel mais recommandé)


    private:
        int selectedIndex;
        sf::Font font;
        std::vector<sf::Text> items;
    };

#endif