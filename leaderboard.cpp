#include "leaderboard.h"

void Leaderboard::updateLeaderboard(const char* playerName, int playerScore) 
{
    int count = 0;

    ifstream inFile("leaderboard.dat", ios::binary);
    if (inFile.is_open()) 
    {
        while (count < MAX_ENTRIES && inFile.read(reinterpret_cast<char*>(&entries[count]), sizeof(Entry))) 
            count++;

        inFile.close();
    }

    // Add new player
    strcpy(entries[count].name, playerName);
    entries[count].score = playerScore;
    count++;

    // Sort entries (descending by score)
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (entries[j].score > entries[i].score) {
                Entry temp = entries[i];
                entries[i] = entries[j];
                entries[j] = temp;
            }
        }
    }

    // Save top 10
    ofstream outFile("leaderboard.dat", ios::binary | ios::trunc);
    for (int i = 0; i < MAX_ENTRIES && i < count; i++)
        outFile.write(reinterpret_cast<char*>(&entries[i]), sizeof(Entry));
        
    outFile.close();
}

void Leaderboard::showLeaderboard() const
{
    
    ifstream inFile("leaderboard.dat", ios::binary);
    if (!inFile)
    {
        DrawString(WIDTH / 2 - 200, HEIGHT / 2 + 50, "NO ONE HAS PLAYED THE GAME YET!", colors[BLACK]);
        return;
    };
    
    DrawString(WIDTH / 2 - 80, HEIGHT / 2 + 50, "LEADERBOARD", colors[BLACK]);
    Entry entry;
    int rank = 1;
    while (inFile.read(reinterpret_cast<char*>(&entry), sizeof(entry))) 
    {
        string str = to_string(rank) + ". " + string(entry.name) + " - " + to_string(entry.score);
        DrawString(WIDTH / 2 - 80, HEIGHT / 2 + 50 - (50 * rank), str, colors[BLACK]);
        rank++;
    }
    inFile.close();
}