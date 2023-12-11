#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

class Property {
public:
    string name;
    int purchasePrice;
    int rent;
    int owner;

    Property(string n, int purchase, int r) : name(n), purchasePrice(purchase), rent(r), owner(-1) {}
};

class Player {
public:
    string name;
    int money;
    int position;
    bool inJail;

    Player(string n) : name(n), money(1500), position(0), inJail(false) {}
};

class MonopolyGame {
private:
    vector<Player> players;
    vector<Property> properties;
    int currentPlayer;
    bool gameSaved;

    void setupProperties() {
    // Standard properties
    properties.push_back(Property("Mediterranean Avenue", 60, 2));
    properties.push_back(Property("Baltic Avenue", 60, 4));
    properties.push_back(Property("Oriental Avenue", 100, 6));
    properties.push_back(Property("Vermont Avenue", 100, 6));
    properties.push_back(Property("Connecticut Avenue", 120, 8));
    properties.push_back(Property("St. Charles Place", 140, 10));
    properties.push_back(Property("States Avenue", 140, 10));
    properties.push_back(Property("Virginia Avenue", 160, 12));
    properties.push_back(Property("St. James Place", 180, 14));
    properties.push_back(Property("Tennessee Avenue", 180, 14));
    properties.push_back(Property("New York Avenue", 200, 16));
    properties.push_back(Property("Kentucky Avenue", 220, 18));
    properties.push_back(Property("Indiana Avenue", 220, 18));
    properties.push_back(Property("Illinois Avenue", 240, 20));
    properties.push_back(Property("Atlantic Avenue", 260, 22));
    properties.push_back(Property("Ventnor Avenue", 260, 22));
    properties.push_back(Property("Marvin Gardens", 280, 24));
    properties.push_back(Property("Pacific Avenue", 300, 26));
    properties.push_back(Property("North Carolina Avenue", 300, 26));
    properties.push_back(Property("Pennsylvania Avenue", 320, 28));
    properties.push_back(Property("Park Place", 350, 35));
    properties.push_back(Property("Boardwalk", 400, 50));

    // Special spaces (we can use dummy values as placeholders)
    properties.push_back(Property("Go", 0, 0)); // Go
    properties.push_back(Property("Jail/Just Visiting", 0, 0)); // Jail/Just Visiting
    properties.push_back(Property("Free Parking", 0, 0)); // Free Parking
    properties.push_back(Property("Go to Jail", 0, 0)); // Go to Jail
    // Add other special spaces or utilities as needed
}

    int rollDice() {
        return (rand() % 6) + 1;
    }

    void movePlayer(Player& player, int roll) {
        if (player.inJail) {
            player.inJail = false; // Simple jail logic: skip one turn
            return;
        }

        player.position = (player.position + roll) % properties.size();
        if (properties[player.position].name == "Go to Jail") {
            player.inJail = true;
            player.position = 0; // Move to "Just Visiting"
            cout << player.name << " goes to jail and is just visiting now!" << endl;
            return;
        }
    }

    void handleProperty(Player& player) {
    Property& property = properties[player.position];

    // Skip handling for non-purchasable spaces
    if (property.purchasePrice == 0) return;

    // Present options to the player
    cout << player.name << " has landed on " << property.name << ".\n";

    if (property.owner == -1) { // Property not owned
        cout << "Would you like to: \n1. Buy for $" << property.purchasePrice 
             << "\n2. Sell a property\n3. Save game for later\nEnter your choice (1/2/3): ";
        int choice;
        cin >> choice;
        switch (choice) {
            case 1: // Buy property
                if (player.money >= property.purchasePrice) {
                    player.money -= property.purchasePrice;
                    property.owner = currentPlayer;
                    cout << player.name << " bought " << property.name << endl;
                } else {
                    cout << "Not enough money to buy this property." << endl;
                }
                break;
            case 2:
                break;
            case 3:
                saveGame();
                cout << "Game saved." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
        }
    } else if (property.owner != currentPlayer) {
        // The property is owned by someone else
        Player& owner = players[property.owner];
        int rent = property.rent;
        player.money -= rent;
        owner.money += rent;
        cout << player.name << " pays $" << rent << " rent for " << property.name << " to " << owner.name << endl;
    }
}



    bool isGameOver() {
        // Simple game over condition: a player runs out of money
        for (const Player& player : players) {
            if (player.money <= 0) return true;
        }
        return false;
    }

public:
    MonopolyGame() : currentPlayer(0) {
        setupProperties();
        srand(time(NULL));
    }

    void addPlayer(string name) {
        players.push_back(Player(name));
    }
    
    void displayBoard() {
        // Top row
        for (int i = 0; i < 11; i++) {
            cout << properties[i].name << " | ";
        }
        cout << endl;

        // Middle rows
        for (int i = 0; i < 9; i++) {
            cout << properties[39 - i].name << " | ";
            for (int j = 0; j < 9; j++) cout << "          | "; // Spacing
            cout << properties[11 + i].name << endl;
        }

        // Bottom row
        for (int i = 0; i < 11; i++) {
            cout << properties[31 - i].name << " | ";
        }
        cout << endl;
    }
    
    void startGame() {
    while (!isGameOver()) { 

        Player& player = players[currentPlayer];
        cout << player.name << "'s turn (Balance: $" << player.money << "). Press enter to roll the dice.";
        cin.ignore();
        int roll = rollDice() + rollDice();
        cout << "You rolled a " << roll << "!" << endl;

        movePlayer(player, roll);
        handleProperty(player);

        // After handling the property, update the player's balance
        cout << player.name << "'s new balance: $" << player.money << endl;

        currentPlayer = (currentPlayer + 1) % players.size();
        if (gameSaved) {
            cout << "Game saved. Exiting the game." << endl;
            break; // Exit the loop if the game has been saved
        }
        
        if (!gameSaved) {
        cout << "Game over!" << endl;
    }
    }

    cout << "Game over!" << endl;
}
void saveGame() {
    ofstream saveFile("monopoly_save.txt");
    if (!saveFile) {
        cerr << "Error saving game." << endl;
        return;
    }

    // Save players
    saveFile << players.size() << endl;
    for (const auto& player : players) {
        saveFile << player.name << " " << player.money << " " << player.position << " " << player.inJail << endl;
    }

    // Save properties
    for (const auto& property : properties) {
        saveFile << property.owner << endl;
    }

    saveFile << currentPlayer << endl;
    gameSaved = true;
    saveFile.close();
}
bool loadGame() {
    ifstream loadFile("monopoly_save.txt");
    if (!loadFile) {
        cerr << "Error loading game." << endl;
        return false;
    }

    // Load players
    size_t numPlayers;
    loadFile >> numPlayers;
    players.clear();
    for (size_t i = 0; i < numPlayers; ++i) {
        string name;
        int money, position;
        bool inJail;
        loadFile >> name >> money >> position >> inJail;
        players.emplace_back(name);
        players.back().money = money;
        players.back().position = position;
        players.back().inJail = inJail;
    }

    // Load properties
    for (auto& property : properties) {
        loadFile >> property.owner;
    }

    loadFile >> currentPlayer;
    loadFile.close();
    return true;
}

};

int main() {
    MonopolyGame game;
    char choice;
    cout << "Do you want to load a saved game? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        if (!game.loadGame()) {
            cout << "Starting a new game..." << endl;
            cout << "Enter number of players (2-8): ";
            int numPlayers;
            cin >> numPlayers;
            cin.ignore(); // To handle newline character after entering number

            for (int i = 0; i < numPlayers; i++) {
                cout << "Enter player " << (i + 1) << "'s name: ";
                string playerName;
                getline(cin, playerName); // Using getline to handle names with spaces
                game.addPlayer(playerName);
            }
        }
    } else {
        cout << "Enter number of players (2-8): ";
        int numPlayers;
        cin >> numPlayers;
        cin.ignore(); // To handle newline character after entering number

        for (int i = 0; i < numPlayers; i++) {
            cout << "Enter player " << (i + 1) << "'s name: ";
            string playerName;
            getline(cin, playerName); // Using getline to handle names with spaces
            game.addPlayer(playerName);
        }
    }

    game.startGame();

    cout << "Do you want to save the game? (y/n): ";
    cin >> choice;
    if (choice == 'y' || choice == 'Y') {
        game.saveGame();
    }

    return 0;
}