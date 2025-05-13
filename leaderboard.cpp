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

    // Sort entries
    for (int i = 0; i < count - 1; i++) 
    {
        for (int j = i + 1; j < count; j++) 
        {
            if (entries[j].score > entries[i].score) 
            {
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
    
    int centerX = WIDTH / 2;
    int startY = HEIGHT / 2 + 130;
   
    if (!inFile) 
    {
        // No data case
        DrawRoundRect(centerX - 220, startY, 440, 60, colors[LIGHT_CORAL], 10);
        DrawString(centerX - 195, startY + 25, "NO ONE HAS PLAYED THE GAME!", colors[WHITE]);
        return;
    }
    
    // Column headers with background
    DrawRoundRect(centerX - 240, startY + 120, 480, 40, colors[SLATE_GRAY], 10);
    DrawString(centerX - 210, startY + 130, "RANK", colors[WHITE]);
    DrawString(centerX - 100, startY + 130, "PLAYER", colors[WHITE]);
    DrawString(centerX + 100, startY + 130, "SCORE", colors[WHITE]);
    
    Entry entry;
    int rank = 1;
    
    while (inFile.read(reinterpret_cast<char*>(&entry), sizeof(entry))) 
    {
        // Row background (alternating colors)
        float* rowColor = (rank % 2 == 0) ? colors[ALICE_BLUE] : colors[WHITE];
        DrawRectangle(centerX - 220, startY - (50 * rank) + 100, 440, 40, rowColor);
        
        // Highlight top 3 players with medals
        if (rank <= 3) 
        {
            float* medalColor;
            if (rank == 1) medalColor = colors[GOLD];        // Gold
            else if (rank == 2) medalColor = colors[SILVER]; // Silver
            else medalColor = colors[PERU];                  // Bronze
            
            DrawCircle(centerX - 210, startY - (50 * rank) + 23 + 100, 15, medalColor);
            DrawString(centerX - 215, startY - (50 * rank) + 15 + 100, Num2Str(rank), colors[WHITE]);
        } 
        else 
        {
            DrawString(centerX - 210, startY - (50 * rank) + 15 + 100, Num2Str(rank), colors[DARK_GRAY]);
        }
        
        // Player name and score
        DrawString(centerX - 100, startY - (50 * rank) + 15 + 100, string(entry.name), colors[BLACK]);
        DrawString(centerX + 100, startY - (50 * rank) + 15 + 100, Num2Str(entry.score), colors[DARK_BLUE]);
        
        rank++;
    }

    // Table column dividers
    DrawLine(centerX - 130, startY + 160, centerX - 130, startY - 400, 1, colors[LIGHT_GRAY]);
    DrawLine(centerX + 80, startY + 160, centerX + 80, startY - 400, 1, colors[LIGHT_GRAY]);
    
    inFile.close();
}