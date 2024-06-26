#pragma once

#include <iostream>
#include <vector>
#include "song/song.h"

// clone hero defaults

class Player {
public:
    int instrument = 0;
    int diff = 0;
    int persistentInst = 0;
    int persistentDiff = 0;
    bool firstReadyUp = true;

    Song songToBeJudged;

    int notesHit = 0;
    int notesMissed = 0;
    int perfectHit = 0;

    bool quit = false;

    float selInstVolume = 0.5;
    float otherInstVolume = 0.375;
    float missVolume = 0.15;

// time in seconds
    float goodFrontend = 0.1f;
    float goodBackend = 0.1f;
    float perfectFrontend = 0.025f;
    float perfectBackend = 0.025f;

    float VideoOffset = (0);
    float InputOffset = 0;

    bool MissHighwayColor = false;

    bool lastNotePerfect = false;

// make the hitwindow bigger for properly doing lifts
    float liftTimingMult = 1.25f;

// 11.5f default --   23.0f 2x
    float defaultHighwayLength = 11.5f;

    float smasherPos = 2.4f; // used to be 2.7

    bool extraGameplayStats = false;

    int notes = 0;
    int combo = 0;
    int maxCombo = 0;
    int score = 0;
    std::vector<int> sustainScoreBuffer{ 0,0,0,0,0 };
    int playerOverhits = 0;

    bool goldStars = false;

    bool overdrive = false;

    bool FC = true;

    float health = 100.0f;

    bool mute = false;

    float xStarThreshold[6] = { 0.05f, 0.175f, 0.325f, 0.5f, 0.7f,  1.0f };

    float overdriveFill = 0.0f;
    float overdriveActiveFill=0.0f;
    double overdriveActiveTime = 0.0;

    float uvOffsetX = 0;
    float uvOffsetY = 0;

    int stars(int baseScore, int difficulty) {
        float starPercent = (float)score/(float)baseScore;
        if (starPercent < xStarThreshold[0]) {return 0;}
        else if (starPercent < xStarThreshold[1]) { return 1; }
        else if (starPercent < xStarThreshold[2]) {return 2;}
        else if (starPercent < xStarThreshold[3]) {return 3;}
        else if (starPercent < xStarThreshold[4]) {return 4;}
        else if (starPercent < xStarThreshold[5]) {return 5;}
        else if (starPercent >= xStarThreshold[5] && difficulty == 3) { goldStars = true; return 5; }
        else return 5;

        return 0;
    }

    int multiplier(int instrument) {
        int od = overdrive ? 2 : 1;

        if (instrument == 1 || instrument == 3){

            if (combo < 10) { uvOffsetX = 0; uvOffsetY = 0 + (overdrive ? 0.5f:0); return 1 * od; }
            else if (combo < 20) { uvOffsetX = 0.25f; uvOffsetY = 0 + (overdrive ? 0.5f : 0);  return 2 * od; }
            else if (combo < 30) { uvOffsetX = 0.5f; uvOffsetY = 0 + (overdrive ? 0.5f : 0);  return 3 * od; }
            else if (combo < 40) { uvOffsetX = 0.75f; uvOffsetY = 0 + (overdrive ? 0.5f : 0); return 4 * od; }
            else if (combo < 50) { uvOffsetX = 0; uvOffsetY = 0.25f + (overdrive ? 0.5f : 0); return 5 * od; }
            else if (combo >= 50) { uvOffsetX = 0.25f; uvOffsetY = 0.25f + (overdrive ? 0.5f : 0); return 6 * od; }
            else { return 1 * od; };
        }
        else {
            if (combo < 10) { uvOffsetX = 0; uvOffsetY = 0 + (overdrive ? 0.5 : 0); return 1 * od; }
            else if (combo < 20) { uvOffsetX = 0.25f; uvOffsetY = 0 + (overdrive ? 0.5 : 0); return 2 * od; }
            else if (combo < 30) { uvOffsetX = 0.5f; uvOffsetY = 0 + (overdrive ? 0.5 : 0); return 3 * od; }
            else if (combo >= 30) { uvOffsetX = 0.75f; uvOffsetY = 0 + (overdrive ? 0.5 : 0); return 4 * od; }
            else { return 1 * od; }
        };
    }

    int maxMultForMeter(int instrument) {
        if (instrument == 1 || instrument == 3)
            return 5;
        else
            return 3;
    }

    float comboFillCalc(int instrument) {
        if (instrument == 0 || instrument == 2) {
            // For instruments 0 and 2, limit the float value to 0.0 to 0.4
            if (combo >= 30) {
                return 1.0f; // If combo is 30 or more, set float value to 1.0
            }
            else {
                return static_cast<float>(combo % 10) / 10.0f; // Float value from 0.0 to 0.9 every 10 notes
            }
        }
        else {
            // For instruments 1 and 3, limit the float value to 0.0 to 0.6
            if (combo >= 50) {
                return 1.0f; // If combo is 50 or more, set float value to 1.0
            }
            else {
                return static_cast<float>(combo % 10) / 10.0f; // Float value from 0.0 to 0.9 every 10 notes
            }
        }
    }




    Color accentColor = Color{255,0,255,255};
    Color overdriveColor = Color{255,200,0,255};

    Color highwayColor = Color{255-64, 0, 255-64,255};
	void resetPlayerStats() {
		notesHit = 0;
		notesMissed = 0;
        perfectHit = 0;
		maxCombo = 0;
		combo = 0;
		score = 0;
		FC = true;
        notes = 0;
        goldStars = false;
        playerOverhits = 0;
        overdrive = false;
        lastNotePerfect = false;
	};

	void HitNote(bool perfect, int instrument) {
		notesHit += 1;
		combo += 1;
		if (combo > maxCombo)
			maxCombo = combo;
		float perfectMult = perfect ? 1.2f : 1.0f;
		score += (int)((30 * (multiplier(instrument)) * perfectMult));
		perfectHit += perfect ? 1 : 0;
        mute = false;
	}
	void HitNoteAudio(bool perfect, int instrument) {
		notesHit += 1;
		combo += 1;
		if (combo > maxCombo)
			maxCombo = combo;
		float perfectMult = perfect ? 1.2f : 1.0f;
		score += (int)((30 * (multiplier(instrument)) * perfectMult));
		perfectHit += perfect ? 1 : 0;
		mute = false;
	}
	void MissNote() {
		notesMissed += 1;
		if (combo > maxCombo)
			maxCombo = combo;
		combo = 0;
		FC = false;
        mute = true;
	}
    void OverHit() {
		if (combo > maxCombo)
			maxCombo = combo;
        combo = 0;
        playerOverhits += 1;
        FC = false;
        mute = true;
    }
};

