#include "views/headers/menu/OptionsMenu.h"
#include "core/headers/AudioManager.h"

OptionsMenu::OptionsMenu(float width, float height)
    : windowWidth(width),
      windowHeight(height),
      selectedIndex(0) {

    activeSlider = -1;
    hoveredSlider = -1;

    font.loadFromFile("assets/fonts/font.ttf");

    std::vector<std::string> names = {
        "General Volume",
        "Music Volume",
        "SFX Volume"
    };

    labels.resize(3);
    sliders.resize(3);

    for (int i = 0; i < 3; ++i) {
        labels[i].setFont(font);
        labels[i].setString(names[i]);
    }

    backText.setFont(font);
    backText.setString("BACK");

    backRect.setFillColor(sf::Color::Transparent);
    backRect.setOutlineThickness(2.f);
    backRect.setOutlineColor(sf::Color(255, 230, 120, 150));

    auto& audio = AudioManager::getInstance();
    sliders[0].setValue(audio.getGeneralVolume());
    sliders[1].setValue(audio.getMusicVolume());
    sliders[2].setValue(audio.getSoundEffectVolume());

    updateLayout();
}

void OptionsMenu::updateLayout() {
    float w = windowWidth;
    float h = windowHeight;

    float startY   = h * 0.30f;
    float spacingY = h * 0.10f;

    float labelX   = w * 0.25f;
    float sliderX  = w * 0.45f;
    float sliderW  = w * 0.35f;
    float sliderH  = h * 0.015f;

    for (int i = 0; i < 3; ++i) {
        labels[i].setCharacterSize(static_cast<unsigned int>(h * 0.04f));
        labels[i].setPosition(labelX, startY + i * spacingY);

        sliders[i].setLayout(
            sliderX,
            startY + i * spacingY + h * 0.01f,
            sliderW,
            sliderH
        );
    }

    backText.setCharacterSize(static_cast<unsigned int>(h * 0.045f));
    sf::FloatRect backBounds = backText.getLocalBounds();

    backText.setOrigin(
        backBounds.left + backBounds.width / 2.f,
        backBounds.top + backBounds.height / 2.f
    );

    backText.setPosition(
        w * 0.5f,
        startY + 3.5f * spacingY
    );

    backRect.setSize({
        backBounds.width + w * 0.04f,
        backBounds.height + h * 0.03f
    });

    backRect.setOrigin(
        backRect.getSize().x / 2.f,
        backRect.getSize().y / 2.f
    );

    backRect.setPosition(backText.getPosition());
}

void OptionsMenu::handleInput(sf::Keyboard::Key key) {
    auto& audio = AudioManager::getInstance();

    if (key == sf::Keyboard::Up && selectedIndex > 0)
        selectedIndex--;

    if (key == sf::Keyboard::Down && selectedIndex < 3)
        selectedIndex++;

    if (selectedIndex < 3) {
        if (key == sf::Keyboard::Left)
            sliders[selectedIndex].decrease(5.f);
        if (key == sf::Keyboard::Right)
            sliders[selectedIndex].increase(5.f);
    }

    if (selectedIndex == 3) {
        backRect.setOutlineColor(sf::Color(255, 230, 120, 150));
    } else {
        backRect.setOutlineColor(sf::Color::Transparent);
    }

    audio.setGeneralVolume(sliders[0].getValue());
    audio.setMusicVolume(sliders[1].getValue());
    audio.setSoundEffectVolume(sliders[2].getValue());

    for (int i = 0; i < 3; ++i)
        labels[i].setFillColor(i == selectedIndex ? sf::Color(255,230,120) : sf::Color::White);

    backText.setFillColor(selectedIndex == 3 ? sf::Color(255,230,120) : sf::Color::White);
}

void OptionsMenu::draw(sf::RenderWindow& window) {
    for (int i = 0; i < 3; ++i) {
        window.draw(labels[i]);
        sliders[i].draw(window);
    }

    window.draw(backRect);
    window.draw(backText);
}

void OptionsMenu::handleMouse(const sf::Event& event, sf::RenderWindow& window) {
    auto& audio = AudioManager::getInstance();
    sf::Vector2f mousePos = window.mapPixelToCoords(
        sf::Mouse::getPosition(window)
    );

    hoveredSlider = -1;

    // Détection hover
    for (int i = 0; i < sliders.size(); ++i) {
        if (sliders[i].contains(mousePos)) {
            hoveredSlider = i;
        }
    }

    // Click slider
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left &&
        hoveredSlider != -1) {

        activeSlider = hoveredSlider;
        selectedIndex = hoveredSlider;
        sliders[activeSlider].updateFromMouse(mousePos);
        }

    // Drag
    if (event.type == sf::Event::MouseMoved && activeSlider != -1) {
        sliders[activeSlider].updateFromMouse(mousePos);
    }

    // Release
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {
        activeSlider = -1;
        }

    // Appliquer volumes
    audio.setGeneralVolume(sliders[0].getValue());
    audio.setMusicVolume(sliders[1].getValue());
    audio.setSoundEffectVolume(sliders[2].getValue());

    // 🎨 Couleur labels
    for (int i = 0; i < labels.size(); ++i) {
        if (i == activeSlider || i == hoveredSlider || i == selectedIndex)
            labels[i].setFillColor(sf::Color(255,230,120));
        else
            labels[i].setFillColor(sf::Color::White);
    }

    bool hoverAnySlider = (hoveredSlider != -1);
    bool hoverBack = isMouseOnBack(mousePos);

    // Gestion de la sélection souris
    if (hoverAnySlider) {
        selectedIndex = hoveredSlider;
    }
    else if (hoverBack) {
        selectedIndex = 3;
    }
    else if (event.type == sf::Event::MouseMoved && activeSlider == -1) {
        selectedIndex = -1;
    }

    // 🎨 BACK visuel
    if (selectedIndex == 3) {
        backText.setFillColor(sf::Color(255, 230, 120));
        backRect.setOutlineColor(sf::Color(255, 230, 120, 150));
    } else {
        backText.setFillColor(sf::Color::White);
        backRect.setOutlineColor(sf::Color::Transparent);
    }
}

int OptionsMenu::getSelectedIndex() const {
    return selectedIndex;
}

bool OptionsMenu::isMouseOnBack(sf::Vector2f mousePos) const {
    return backText.getGlobalBounds().contains(mousePos);
}

bool OptionsMenu::buttonBackClicked(const sf::Event& event, sf::RenderWindow& window) const {
    if (event.type == sf::Event::MouseButtonReleased &&
        event.mouseButton.button == sf::Mouse::Left) {

        sf::Vector2f mousePos = window.mapPixelToCoords(
            sf::Mouse::getPosition(window)
        );

        return backText.getGlobalBounds().contains(mousePos);
        }
    return false;
}
