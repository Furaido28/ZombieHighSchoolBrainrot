#include "../headers/Player.h"
#include <algorithm> // Pour std::max

// Constructeur : Initialisation des variables
Player::Player()
    : position(0.f, 0.f),
      size(48.f, 48.f),
      currentDirection(Direction::Down),
      moving(false),
      attackCooldownTimer(0.f) // Init du cooldown à 0
{
    maxHealth = 100;
    health = maxHealth;
}

void Player::takeDamage(int amount){
    if (invincibilityTimer > 0.f)
        return;

    health -= amount;
    if (health < 0)
        health = 0;

    invincibilityTimer = 3.f; // 3s d’invincibilité
}

int Player::getHealth() const {
    return health;
}

int Player::getMaxHealth() const {
    return maxHealth;
}

void Player::setHealth(int value) {
    health = 1;
}

bool Player::isInvincible() const{
    return invincibilityTimer > 0.f;
}

bool Player::isAlive() const {
    return health > 0;
}

void Player::move(const sf::Vector2f& delta) {
    position += delta;
}

void Player::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);
}

void Player::setSize(float w, float h) {
    size = sf::Vector2f(w, h);
}

// Mise à jour : Gère les timers (Invincibilité + Attaque)
void Player::update(float dt) {
    if (invincibilityTimer > 0.f)
        invincibilityTimer -= dt;

    if (attackCooldownTimer > 0.f)
        attackCooldownTimer -= dt;
}

sf::Vector2f Player::getPosition() const {
    return position;
}

sf::Vector2f Player::getSize() const {
    return size;
}

float Player::getRadius() const {
    return std::min(size.x, size.y) * 0.5f;
}

sf::FloatRect Player::getGlobalBounds() const {
    float halfWidth = size.x / 2.f;
    float halfHeight = size.y / 2.f;

    return sf::FloatRect(
        position.x - halfWidth,
        position.y - halfHeight,
        size.x,
        size.y
    );
}

// --- GESTION DE LA DIRECTION ---
void Player::setDirection(Direction dir) {
    currentDirection = dir;
}

Direction Player::getDirection() const {
    return currentDirection;
}

void Player::setMoving(bool m) { moving = m; }

bool Player::isMoving() const { return moving; }

Inventory &Player::getInventory() { return inventory; }

// --- NOUVELLE LOGIQUE D'ATTAQUE ---
AttackInfo Player::tryAttack() {
    AttackInfo info;
    info.valid = false; // Par défaut, pas d'attaque

    if (!attackRequested) return info;

    attackRequested = false;

    // 1. Vérification du Cooldown
    if (attackCooldownTimer > 0.f) {
        return info;
    }

    // 2. Récupération de l'arme depuis l'inventaire
    int slotIndex = inventory.getSelectedSlot();
    auto& slots = inventory.getSlots();

    // Vérifie si le slot est valide et contient un item
    if (slotIndex < 0 || slotIndex >= (int)slots.size() || !slots[slotIndex].has_value()) {
        return info; // Pas d'item en main
    }

    // Référence à l'item
    const Item& item = slots[slotIndex].value();

    // Vérifie si c'est bien une arme
    if (item.type != ItemType::Weapon) {
        return info;
    }

    // 3. Initialisation de l'attaque valide
    info.valid = true;
    info.damage = item.damage;
    info.isProjectile = item.isProjectile;

    // On applique le cooldown de l'arme
    attackCooldownTimer = item.cooldown;

    // 4. Calcul du vecteur de direction (Où regarde le joueur ?)
    sf::Vector2f dirVec(0.f, 0.f);
    switch (currentDirection) {
        case Direction::Up:    dirVec = {0.f, -1.f}; break;
        case Direction::Down:  dirVec = {0.f, 1.f};  break;
        case Direction::Left:  dirVec = {-1.f, 0.f}; break;
        case Direction::Right: dirVec = {1.f, 0.f};  break;
    }

    // 5. Configuration selon le type d'attaque
    if (info.isProjectile) {
        // --- PROJECTILE ---
        // Le tir part du centre du joueur
        info.startPosition = position;
        info.velocity = dirVec * item.projectileSpeed;
    }
    else {
        // --- MELEE (Corps à corps) ---
        // On définit une zone rectangulaire devant le joueur
        float reach = item.range;  // La "longueur" du coup
        float width = 40.f;        // La "largeur" du coup (peut être hardcodé ou dans l'item)

        // Calcul du centre de la Hitbox d'attaque
        // On part du centre du joueur et on avance dans la direction
        // Distance = (Demi-taille joueur) + (Demi-portée arme) pour que la hitbox soit collée devant
        float offset = (std::max(size.x, size.y) / 2.f) + (reach / 2.f);
        sf::Vector2f hitCenter = position + (dirVec * offset);

        // Dimensions de la boîte selon l'orientation
        float boxW, boxH;
        if (currentDirection == Direction::Up || currentDirection == Direction::Down) {
            // Attaque verticale : la hitbox est large (width) et haute (reach)
            boxW = width;
            boxH = reach;
        } else {
            // Attaque horizontale : la hitbox est large (reach) et haute (width)
            boxW = reach;
            boxH = width;
        }

        info.meleeHitbox = sf::FloatRect(
            hitCenter.x - boxW / 2.f,
            hitCenter.y - boxH / 2.f,
            boxW,
            boxH
        );
    }

    return info;
}
void Player::addMovement(const sf::Vector2f& dir) {
    movementIntent += dir;
}

sf::Vector2f Player::consumeMovement() {
    sf::Vector2f result = movementIntent;
    movementIntent = {0.f, 0.f};
    return result;
}

void Player::requestAttack() {
    attackRequested = true;
}