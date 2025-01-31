#include "useful.hpp"

// public
sf::Time dt(sf::Clock clock)
{
    return clock.restart();
}


TileMap::TileMap(sf::RenderWindow *win)
{
	window = win;

	// Create a cell
    cell = sf::RectangleShape(sf::Vector2f(64.0f, 64.0f));
    cell.setFillColor(sf::Color::Transparent);  // Fill color transparent
    cell.setOutlineThickness(1);  // Set outline thickness
    cell.setOutlineColor(sf::Color::Transparent);  // Set outline color
}

bool TileMap::load(const std::filesystem::path& tileset, sf::Vector2u tileSize, int* tiles, char* tiles_c, unsigned int width, unsigned int height)
{
    // Load the tileset texture
    if (!m_tileset.loadFromFile(tileset))
        return false;

    // Resize the vertex array to fit the level size
    m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
    m_vertices.resize(width * height * 6);

    // Populate the vertex array, with two triangles per tile
    for (unsigned int i = 0; i < width; ++i)
    {
        for (unsigned int j = 0; j < height; ++j)
        {
            // Get the current tile number
            const int tileNumber = tiles[i + j * width];

            // Find its position in the tileset texture
            const int tu = tileNumber % (m_tileset.getSize().x / 64); // Original tile size (assuming 64x64 tiles in the texture)
            const int tv = tileNumber / (m_tileset.getSize().x / 64); // Original tile size

            // Get a pointer to the triangles' vertices of the current tile
            sf::Vertex* triangles = &m_vertices[(i + j * width) * 6];

            // Define the 6 corners of the two triangles
            triangles[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
            triangles[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[2].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
            triangles[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);
            triangles[4].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
            triangles[5].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);

            // Define the 6 matching texture coordinates for the original 64x64 tiles
            triangles[0].texCoords = sf::Vector2f(tu * 64, tv * 64); // Original texture coordinates
            triangles[1].texCoords = sf::Vector2f((tu + 1) * 64, tv * 64);
            triangles[2].texCoords = sf::Vector2f(tu * 64, (tv + 1) * 64);
            triangles[3].texCoords = sf::Vector2f(tu * 64, (tv + 1) * 64);
            triangles[4].texCoords = sf::Vector2f((tu + 1) * 64, tv * 64);
            triangles[5].texCoords = sf::Vector2f((tu + 1) * 64, (tv + 1) * 64);
        }
    }

	cells.clear();  // Clear the cells vector

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            if (tiles_c[i + j * 10] != '_') {
                // Access cells array (1D) with 2D indices (same index logic)
                //cell.setFillColor(sf::Color(255, 0, 0, 100));
            }
            else {
                cell.setFillColor(sf::Color::Transparent);  // Fill color transparent
            }

            if (tiles_c[i + j * 10] == 'd') {
                cell.setScale({ 1.0f, 0.5f });
                cell.setPosition({ i * 64.0f, j * 64.0f });
            }
            else if (tiles_c[i + j * 10] == 'h') {
                cell.setScale({ 1.0f, 0.2f });
                cell.setPosition({ i * 64.0f, j * 64.0f  + 54.0f});
            }
            else if (tiles_c[i + j * 10] == 'f') {
                cell.setScale({ 1.0f, 0.6f });
                cell.setPosition({ i * 64.0f, j * 64.0f + 30.f });
            }
            else {
                cell.setScale({ 1.0f, 1.0f });
                cell.setPosition({ i * 64.0f, j * 64.0f });
            }

            // Position the cell at (i * gridCellSize, j * gridCellSize)
            cells.push_back({
                i + j * 10,
                cell,
                tiles_c[i + j * 10] != '_',// Set collidable to true if the cell is collidable
                tiles_c[i + j * 10]
                });
        }
    }


    return true;
}

int* TileMap::processFileAndReturnInts(const std::string& filename, size_t& size, Maps *maps) {
    std::ifstream inputFile(filename);  // Open the input file
    if (!inputFile) {
        std::cerr << "Error opening the file." << std::endl;
        return nullptr;
    }

    std::string line, fileContent;
    size_t lineCount = 0;

    // Skip the first 11 lines and start processing from line 12
    size_t x = maps->y * 20;

    while (std::getline(inputFile, line) && lineCount < x) {
        lineCount++;  // Increment the line count to skip the first 11 lines
    }

    // Now read the next 10 lines (or fewer if the file ends) into a single string
    size_t linesRead = 0;
    while (std::getline(inputFile, line) && linesRead < 10) {
        fileContent += line + " ";  // Concatenate the lines (space-separated)
        linesRead++;
    }

    // Close the file
    inputFile.close();

    // Remove whitespace and split by commas
    std::string temp;
    std::vector<int> numbers;
    std::istringstream stream(fileContent);

    numbers.clear();

    while (std::getline(stream, temp, ',')) {
        // Remove any surrounding whitespace
        temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());

        if (!temp.empty()) {
            numbers.push_back(std::stoi(temp)); // Convert to int and add to result vector
        }
    }

    // Dynamically allocate memory for int*
    size = numbers.size();
    int* intArray = new int[size];

    // Copy the vector data to the int array
    for (size_t i = 0; i < size; ++i) {
        intArray[i] = numbers[i];
    }

    return intArray; // Return the dynamically allocated array
}

// Function to process the file and return a dynamically allocated char array starting from line 12
char* TileMap::processFileAndReturnChars(const std::string& filename, size_t& size, Maps* maps) {
    std::ifstream inputFile(filename);  // Open the input file
    if (!inputFile) {
        std::cerr << "Error opening the file." << std::endl;
        return nullptr;
    }

    std::string line = "", fileContent = "";
    size_t lineCount = 0;

    size_t x = maps->y * 20 + 10;

    // Skip the first 11 lines and start processing from line 12
    while (std::getline(inputFile, line) && lineCount < x) {
        lineCount++;  // Increment the line count to skip the first 11 lines
    }

    // Now read the next 10 lines (or fewer if the file ends) into a single string
    size_t linesRead = 0;
    while (std::getline(inputFile, line) && linesRead < 10) {
        fileContent += line + " ";  // Concatenate the lines (space-separated)
        linesRead++;
    }

    // Close the file
    inputFile.close();

    // Remove whitespace and split by commas
    std::string temp = "";
    std::vector<char> charArray;

    std::istringstream stream(fileContent);

    while (std::getline(stream, temp, ',')) {
        // Remove any surrounding whitespace
        temp.erase(remove_if(temp.begin(), temp.end(), ::isspace), temp.end());

        if (!temp.empty()) {
            // Assuming each element is a single character, add the first character
            charArray.push_back(temp[0]);
        }
    }

	charArray.push_back('\0'); // Add null terminator at the end

    // Dynamically allocate memory for char*
    size = charArray.size();
    char* charPtr = new char[size];

    // Copy the vector data to the char array
    for (size_t i = 0; i < size; ++i) {
        charPtr[i] = charArray[i];
    }

    // Ensure that the previous collision map data doesn't overlap
    // This is achieved by properly managing memory through new allocation.
    // You can clear/clean up memory of previous collision maps if necessary elsewhere in your class.

    return charPtr; // Return the dynamically allocated array
}



// Private
void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // Apply the transform
    states.transform *= getTransform();

    // Apply the tileset texture
    states.texture = &m_tileset;

    // Draw the vertex array
    target.draw(m_vertices, states);

    for (auto c : cells)
        window->draw(c.shape);  // Draw the cell

}


AnimatableSprite::AnimatableSprite(const sf::Texture& texture, int frameWidth, int frameHeight) : m_frameWidth(frameWidth), m_frameHeight(frameHeight), sf::Sprite::Sprite(texture)
{
	setTexture(texture);
}

void AnimatableSprite::addAnimation(const std::string& name, const std::vector<sf::IntRect>& frames, float frameRate)
{
    m_animations[name] = { frames, frameRate, sf::Time::Zero, 0 };
}

void AnimatableSprite::switchAnimation(const std::string& name)
{
    if (m_animations.find(name) != m_animations.end()) {
        m_currentAnimation = name;
        m_animations[name].elapsedTime = sf::Time::Zero; // Reset time for new animation
        m_animations[name].currentFrame = 0; // Reset to the first frame
        setTextureRect(m_animations[name].frames[0]); // Set initial frame
    }
}

void AnimatableSprite::update(sf::Time deltaTime) {
    // DeltaTime
    //if (m_currentAnimation.empty()) {
    //    return; // No animation set yet
    //}

    //Animation& animation = m_animations[m_currentAnimation];
    //animation.elapsedTime += deltaTime;

    //// Change frame if enough time has passed
    //if (animation.elapsedTime >= sf::seconds(1.f / animation.frameRate)) {
    //    animation.elapsedTime -= sf::seconds(1.f / animation.frameRate);
    //    animation.currentFrame = (animation.currentFrame + 1) % animation.frames.size();
    //    setTextureRect(animation.frames[animation.currentFrame]);
    //}

    // No deltatime
    if (m_currentAnimation.empty()) {
        return; // No animation set yet
    }

    Animation& animation = m_animations[m_currentAnimation];

    // Increment the frame counter
    animation.frameCounter++;

    // If enough frames have passed, change the frame
    if (animation.frameCounter >= animation.frameRate) {
        animation.frameCounter = 0;
        animation.currentFrame = (animation.currentFrame + 1) % animation.frames.size();
        setTextureRect(animation.frames[animation.currentFrame]);
    }
}