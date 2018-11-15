/**
 * reference code
 *    http://www.xtronical.com/
 */
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>

#define OLED_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define FIRE_BUT 16
#define RIGHT_BUT 17
#define LEFT_BUT 4

#define MOTHERSHIP_HEIGHT 4
#define MOTHERSHIP_WIDTH 16
#define MOTHERSHIP_SPEED 2
#define MOTHERSHIP_SPAWN_CHANCE 250
#define DISPLAY_MOTHERSHIP_BONUS_TIME 20

#define ALIEN_HEIGHT 8
#define NUM_ALIEN_COLUMNS 7
#define NUM_ALIEN_ROWS 3
#define X_START_OFFSET 6
#define SPACE_BETWEEN_ALIEN_COLUMNS 5
#define LARGEST_ALIEN_WIDTH 11
#define SPACE_BETWEEN_ROWS 9
#define INVADERS_DROP_BY 4
#define INVADERS_SPEED 12
#define INVADER_HEIGHT 8
#define EXPLOSION_GFX_TIME 7
#define INVADERS_Y_START MOTHERSHIP_HEIGHT - 1
#define AMOUNT_TO_DROP_BY_PER_LEVEL 4
#define LEVEL_TO_RESET_TO_START_HEIGHT 4
#define ALIEN_X_MOVE_AMOUNT 1
#define CHANCEOFBOMBDROPPING 20
#define BOMB_HEIGHT 4
#define BOMB_WIDTH 2
#define MAXBOMBS 3

#define TANKGFX_WIDTH 13
#define TANKGFX_HEIGHT 8
#define PLAYER_X_MOVE_AMOUNT 2
#define LIVES 3
#define PLAYER_EXPLOSION_TIME 10
#define PLAYER_Y_START 56
#define PLAYER_X_START 0

#define MISSILE_HEIGHT 4
#define MISSILE_WIDTH 1
#define MISSILE_SPEED 4

#define ACTIVE 0
#define EXPLODING 1
#define DESTROYED 2

const unsigned char MotherShipGfx [] PROGMEM = {
  B00111111,B11111100,
  B01101101,B10110110,
  B11111111,B11111111,
  B00111001,B10011100
};

const unsigned char InvaderTopGfx [] PROGMEM = {
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B00100100,
  B01011010,
  B10100101
};

const unsigned char InvaderTopGfx2 [] PROGMEM = {
  B00011000,
  B00111100,
  B01111110,
  B11011011,
  B11111111,
  B01011010,
  B10000001,
  B01000010
};

static const unsigned char PROGMEM InvaderMiddleGfx [] = {
  B00100000,B10000000,
  B00010001,B00000000,
  B00111111,B10000000,
  B01101110,B11000000,
  B11111111,B11100000,
  B10111111,B10100000,
  B10100000,B10100000,
  B00011011,B00000000
};

static const unsigned char PROGMEM InvaderMiddleGfx2 [] = {
  B00100000,B10000000,
  B00010001,B00000000,
  B10111111,B10100000,
  B10101110,B10100000,
  B11111111,B11100000,
  B00111111,B10000000,
  B00100000,B10000000,
  B01000000,B01000000
};

static const unsigned char PROGMEM InvaderBottomGfx [] = {
  B00001111,B00000000,
  B01111111,B11100000,
  B11111111,B11110000,
  B11100110,B01110000,
  B11111111,B11110000,
  B00111001,B11000000,
  B01100110,B01100000,
  B00110000,B11000000
};

static const unsigned char PROGMEM InvaderBottomGfx2 [] = {
  B00001111,B00000000,
  B01111111,B11100000,
  B11111111,B11110000,
  B11100110,B01110000,
  B11111111,B11110000,
  B00111001,B11000000,
  B01000110,B00100000,
  B10000000,B00010000
};

static const unsigned char PROGMEM ExplosionGfx [] = {
  B00001000,B10000000,
  B01000101,B00010000,
  B00100000,B00100000,
  B00010000,B01000000,
  B11000000,B00011000,
  B00010000,B01000000,
  B00100101,B00100000,
  B01001000,B10010000
};

const unsigned char PROGMEM TankGfx [] = {
  B00000010,B00000000,
  B00000111,B00000000,
  B00000111,B00000000,
  B01111111,B11110000,
  B11111111,B11111000,
  B11111111,B11111000,
  B11111111,B11111000,
  B11111111,B11111000,
};

static const unsigned char PROGMEM MissileGfx [] = {
  B10000000,
  B10000000,
  B10000000,
  B10000000
};

static const unsigned char PROGMEM AlienBombGfx [] = {
  B10000000,
  B01000000,
  B10000000,
  B01000000
};

struct GameObjectStruct {
  signed int X;
  signed int Y;
  unsigned char Status; // 0 active, 1 exploding, 2 destroyed
};

struct AlienStruct {
  GameObjectStruct Ord;
  unsigned char ExplosionGfxCounter;
};

struct PlayerStruct {
  GameObjectStruct Ord;
  unsigned int Score;
  unsigned char Lives;

  unsigned char Level;
  unsigned char AliensDestroyed;
  unsigned char AlienSpeed;
  unsigned char ExplosionGfxCounter;
};

Adafruit_SSD1306 display(1);

AlienStruct Alien[NUM_ALIEN_COLUMNS][NUM_ALIEN_ROWS];
AlienStruct MotherShip;
GameObjectStruct AlienBomb[MAXBOMBS];

static const int TOTAL_ALIENS = NUM_ALIEN_COLUMNS * NUM_ALIEN_ROWS;

byte AlienWidth[] = {8, 11, 12};

char AlienXMoveAmount = 2;
signed char InvadersMoveCounter;
bool AnimationFrame = false;

signed char MotherShipSpeed;
unsigned int MotherShipBonus;
signed int MotherShipBonusXPos;
unsigned char MotherShipBonusCounter;

PlayerStruct Player;
GameObjectStruct Missile;

unsigned int HiScore;
bool GameInPlay = false;

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
  InitAliens(0);
  InitPlayer();

  pinMode(RIGHT_BUT, INPUT_PULLUP);
  pinMode(LEFT_BUT, INPUT_PULLUP);
  pinMode(FIRE_BUT, INPUT_PULLUP);

  display.setTextSize(1);
  display.setTextColor(WHITE);

  EEPROM.get(0, HiScore);
  if (HiScore == 65535) {
    HiScore = 0;
    EEPROM.put(0, HiScore);
  }
}

void loop() {
  if (GameInPlay) {
    Physics();
    UpdateDisplay();
  } else {
    AttractScreen();
  }
}

void AttractScreen() {
  display.clearDisplay();
  CentreText("Play", 0);
  CentreText("Space Invaders", 12);
  CentreText("Press Fire to start", 24);
  CentreText("Hi Score     ", 36);
  display.setCursor(80, 36);
  display.print(HiScore);
  display.display();
  if (digitalRead(FIRE_BUT) == HIGH) {
    GameInPlay = true;
    NewGame();
  }

  if ((digitalRead(LEFT_BUT) == HIGH) & (digitalRead(RIGHT_BUT) == HIGH)) {
    HiScore = 0;
    EEPROM.put(0, HiScore);
  }
}

void Physics() {
  if (Player.Ord.Status == ACTIVE) {
    AlienControl();
    MotherShipPhysics();
    PlayerControl();
    MissileControl();
    CheckCollisions();
  }
}

unsigned char GetScoreForAlien(int RowNumber) {
  switch (RowNumber) {
    case 0: return 30;
    case 1: return 20;
    case 2: return 10;
  }
}

void MotherShipPhysics() {
  if (MotherShip.Ord.Status == ACTIVE) {
    MotherShip.Ord.X += MotherShipSpeed;
    if (MotherShipSpeed > 0) {
      if (MotherShip.Ord.X >= SCREEN_WIDTH) {
        MotherShip.Ord.Status = DESTROYED; 
      }
    } else {
      if (MotherShip.Ord.X + MOTHERSHIP_WIDTH < 0) {
        MotherShip.Ord.Status = DESTROYED;
      }
    }
  } else {
    if (random(MOTHERSHIP_SPAWN_CHANCE) == 1) {
      MotherShip.Ord.Status = ACTIVE;
      if (random(2) == 1) {
        MotherShip.Ord.X = SCREEN_WIDTH;
        MotherShipSpeed = -MOTHERSHIP_SPEED;
      } else {
        MotherShip.Ord.X = -MOTHERSHIP_WIDTH;
        MotherShipSpeed = MOTHERSHIP_SPEED;
      }
    }
  }
}

void PlayerControl() {
  if ((digitalRead(RIGHT_BUT) == HIGH) & (Player.Ord.X + TANKGFX_WIDTH < SCREEN_WIDTH)) {
    Player.Ord.X += PLAYER_X_MOVE_AMOUNT;
  }
  if ((digitalRead(LEFT_BUT) == HIGH) & (Player.Ord.X > 0)) {
    Player.Ord.X -= PLAYER_X_MOVE_AMOUNT;
  }
  if ((digitalRead(FIRE_BUT) == HIGH) & (Missile.Status != ACTIVE)) {
    Missile.X = Player.Ord.X + (TANKGFX_WIDTH / 2);
    Missile.Y = PLAYER_Y_START;
    Missile.Status = ACTIVE;
  }
}

void MissileControl() {
  if (Missile.Status == ACTIVE) {
    Missile.Y -= MISSILE_SPEED;
    if (Missile.Y + MISSILE_HEIGHT < 0) {
      Missile.Status = DESTROYED;
    }
  }
}

void AlienControl() {
  if ((InvadersMoveCounter--) < 0) {
    bool Dropped = false;
    if ((RightMostPos() + AlienXMoveAmount >= SCREEN_WIDTH) | (LeftMostPos() + AlienXMoveAmount < 0)) {
      AlienXMoveAmount = -AlienXMoveAmount; // reverse direction
      Dropped = true;
    }

    for (int across = 0; across < NUM_ALIEN_COLUMNS; across++) {
      for (int down = 0; down < NUM_ALIEN_ROWS; down++) {
        if (Alien[across][down].Ord.Status == ACTIVE) {
          if (Dropped == false) {
            Alien[across][down].Ord.X += AlienXMoveAmount;
          } else {
            Alien[across][down].Ord.Y += INVADERS_DROP_BY;
          }
        }
      }
    }
    InvadersMoveCounter = Player.AlienSpeed;
    AnimationFrame = !AnimationFrame;
  }

  if (random(CHANCEOFBOMBDROPPING) == 1) {
    DropBomb();
  }
  MoveBombs();
}

void MoveBombs() {
  for (int i = 0; i < MAXBOMBS; i++) {
    if (AlienBomb[i].Status == ACTIVE) {
      AlienBomb[i].Y += 2;
    }
  }
}

void DropBomb() {
  bool Free = false;
  unsigned char ActiveCols[NUM_ALIEN_COLUMNS];
  unsigned char BombIdx = 0;

  while ((Free == false) & (BombIdx < MAXBOMBS)) {
    if (AlienBomb[BombIdx].Status == DESTROYED) {
      Free = true;
    } else {
      BombIdx++;
    }
  }
  if (Free) {
    unsigned char Columns = 0;
    unsigned char ActiveColCount = 0;
    signed char Row;
    unsigned char ChosenColumn;

    while (Columns < NUM_ALIEN_COLUMNS) {
      Row = 2;
      while (Row >= 0) {
        if (Alien[Columns][Row].Ord.Status == ACTIVE) {
          ActiveCols[ActiveColCount] = Columns;
          ActiveColCount++;
          break;
        }
        Row--;
      }
      Columns++;
    }

    ChosenColumn = random(ActiveColCount);
    Row = 2;
    while (Row >= 0) {
      if (Alien[ActiveCols[ChosenColumn]][Row].Ord.Status == ACTIVE) {
        AlienBomb[BombIdx].Status = ACTIVE;
        AlienBomb[BombIdx].X = Alien[ActiveCols[ChosenColumn]][Row].Ord.X + int(AlienWidth[Row] / 2);
        AlienBomb[BombIdx].X = (AlienBomb[BombIdx].X - 2) + random(0, 4);
        AlienBomb[BombIdx].Y = Alien[ActiveCols[ChosenColumn]][Row].Ord.Y + 4;
        break;
      }
      Row--;
    }
  }  
}

void BombCollisions() {
  for (int i = 0; i < MAXBOMBS; i++) {
    if (AlienBomb[i].Status == ACTIVE) {
      if (AlienBomb[i].Y > 64) {
        AlienBomb[i].Status = DESTROYED;
      } else {
        if (Collision(AlienBomb[i], BOMB_WIDTH, BOMB_HEIGHT, Missile, MISSILE_WIDTH, MISSILE_HEIGHT)) {
          AlienBomb[i].Status = EXPLODING;
          Missile.Status = DESTROYED;
        } else {
          if (Collision(AlienBomb[i], BOMB_WIDTH, BOMB_HEIGHT, Player.Ord, TANKGFX_WIDTH, TANKGFX_HEIGHT)) {
            PlayerHit();
            AlienBomb[i].Status = DESTROYED;
          }
        }
      }
    }
  }
}

void PlayerHit() {
  Player.Ord.Status = EXPLODING;
  Player.ExplosionGfxCounter = PLAYER_EXPLOSION_TIME;
  Missile.Status = DESTROYED;
}

void CheckCollisions() {
  MissileAndAlienCollisions();
  MotherShipCollisions();
  BombCollisions();
}

void MotherShipCollisions() {
  if ((Missile.Status == ACTIVE) & (MotherShip.Ord.Status == ACTIVE)) {
    if (Collision(Missile, MISSILE_WIDTH, MISSILE_HEIGHT, MotherShip.Ord, MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT)) {
      MotherShip.Ord.Status = EXPLODING;
      MotherShip.ExplosionGfxCounter = EXPLOSION_GFX_TIME;
      Missile.Status = DESTROYED;

      MotherShipBonus = random(4);
      switch (MotherShipBonus) {
        case 0: MotherShipBonus = 50; break;
        case 1: MotherShipBonus = 100; break;
        case 2: MotherShipBonus = 150; break;
        case 3: MotherShipBonus = 300; break;
      }
      Player.Score += MotherShipBonus;
      MotherShipBonusXPos = MotherShip.Ord.X;
      if (MotherShipBonusXPos > 100) {
        MotherShipBonusXPos = 100;
      }
      if (MotherShipBonusXPos < 0) {
        MotherShipBonusXPos = 0;
      }
      MotherShipBonusCounter = DISPLAY_MOTHERSHIP_BONUS_TIME;
    }
  }
}

void MissileAndAlienCollisions() {
  for (int across = 0; across < NUM_ALIEN_COLUMNS; across++) {
    for (int down = 0; down < NUM_ALIEN_ROWS; down++) {
      if (Alien[across][down].Ord.Status == ACTIVE) {
        if (Missile.Status == ACTIVE) {
          if (Collision(Missile, MISSILE_WIDTH, MISSILE_HEIGHT, Alien[across][down].Ord, AlienWidth[down], INVADER_HEIGHT)) {
            Alien[across][down].Ord.Status = EXPLODING;
            Missile.Status = DESTROYED;
            Player.Score += GetScoreForAlien(down);
            Player.AliensDestroyed++;

            Player.AlienSpeed = ((1 - (Player.AliensDestroyed / (float)TOTAL_ALIENS)) * INVADERS_SPEED);

            if (Player.AliensDestroyed == TOTAL_ALIENS - 2) {
              if (AlienXMoveAmount > 0) {
                AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT * 2;
              } else {
                AlienXMoveAmount = -(ALIEN_X_MOVE_AMOUNT * 2);
              }
            }
            if (Player.AliensDestroyed == TOTAL_ALIENS - 1) {
              if (AlienXMoveAmount > 0) {
                AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT * 4;
              } else {
                AlienXMoveAmount = -(ALIEN_X_MOVE_AMOUNT * 4);
              }
            }
            if (Player.AliensDestroyed == TOTAL_ALIENS) {
              NextLevel(&Player);
            }
          }
          if (Alien[across][down].Ord.Status == ACTIVE) {
            if (Collision(Player.Ord, TANKGFX_WIDTH, TANKGFX_HEIGHT, Alien[across][down].Ord, AlienWidth[down], ALIEN_HEIGHT)) {
              PlayerHit();
            } else {
              if (Alien[across][down].Ord.Y + 8 > SCREEN_HEIGHT) {
                PlayerHit();
              }
            }
          }
        }
      }
    }
  }
}

bool Collision(GameObjectStruct Obj1, unsigned char Width1, unsigned char Height1, GameObjectStruct Obj2, unsigned char Width2, unsigned char Height2) {
  return ((Obj1.X + Width1 > Obj2.X) & (Obj1.X < Obj2.X + Width2) & (Obj1.Y + Height1 > Obj2.Y) & (Obj1.Y < Obj2.Y + Height2));
}

int RightMostPos() {
  int across = NUM_ALIEN_COLUMNS - 1;
  int down;
  int largest = 0;
  int rightpos;
  while (across >= 0) {
    down = 0;
    while (down < NUM_ALIEN_ROWS) {
      if (Alien[across][down].Ord.Status == ACTIVE) {
        rightpos = Alien[across][down].Ord.X + AlienWidth[down];
        if (rightpos > largest) {
          largest = rightpos;
        }
        down++;
      }
      if (largest > 0) {
        return largest;
      }
      across--;
    }
  }
  return 0;
}

int LeftMostPos() {
  int across = 0;
  int down;
  int smallest = SCREEN_WIDTH * 2;
  while (across < NUM_ALIEN_COLUMNS) {
    down = 0;
    while (down < NUM_ALIEN_ROWS) {
      if (Alien[across][down].Ord.Status == ACTIVE) {
        if (Alien[across][down].Ord.X < smallest) {
          smallest = Alien[across][down].Ord.X;
        }
        down++;
      }
      if (smallest < SCREEN_WIDTH * 2) {
        return smallest;
      }
      across++;
    }
  }
  return 0;
}

void UpdateDisplay() {
  int i;
  
  display.clearDisplay();

  if (MotherShipBonusCounter > 0) {
    display.setCursor(MotherShipBonusXPos, 0);
    display.print(MotherShipBonus);
    MotherShipBonusCounter--;
  } else {
    display.setCursor(0, 0);
    display.print(Player.Score);
    display.setCursor(SCREEN_WIDTH - 7, 0);
    display.print(Player.Lives);
  }

  for (i = 0; i < MAXBOMBS; i++) {
    if (AlienBomb[i].Status == ACTIVE) {
      display.drawBitmap(AlienBomb[i].X, AlienBomb[i].Y, AlienBombGfx, 2, 4, WHITE);
    } else {
      if (AlienBomb[i].Status == EXPLODING) {
        display.drawBitmap(AlienBomb[i].X - 4, AlienBomb[i].Y, ExplosionGfx, 4, 8, WHITE);
      }
      AlienBomb[i].Status = DESTROYED;
    }
  }

  for (int across = 0; across < NUM_ALIEN_COLUMNS; across++) {
    for (int down = 0; down < NUM_ALIEN_ROWS; down++) {
      if (Alien[across][down].Ord.Status == ACTIVE) {
        switch (down) {
          case 0:
            if (AnimationFrame) {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderTopGfx, AlienWidth[down], INVADER_HEIGHT, WHITE);
            } else {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderTopGfx2, AlienWidth[down], INVADER_HEIGHT, WHITE);
            }
            break;
          case 1:
            if (AnimationFrame) {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderMiddleGfx, AlienWidth[down], INVADER_HEIGHT, WHITE);
            } else {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderMiddleGfx2, AlienWidth[down], INVADER_HEIGHT, WHITE);
            }
            break;
          default:
            if (AnimationFrame) {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderBottomGfx, AlienWidth[down], INVADER_HEIGHT, WHITE);
            } else {
              display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, InvaderBottomGfx2, AlienWidth[down], INVADER_HEIGHT, WHITE);
            }
            break;
        }
      } else {
        if (Alien[across][down].Ord.Status == EXPLODING) {
          Alien[across][down].ExplosionGfxCounter--;
          if (Alien[across][down].ExplosionGfxCounter > 0) {
            display.drawBitmap(Alien[across][down].Ord.X, Alien[across][down].Ord.Y, ExplosionGfx, 13, 8, WHITE);
          } else {
            Alien[across][down].Ord.Status = DESTROYED;
          }
        }
      }
    }
  }

  if (Player.Ord.Status == ACTIVE) {
    display.drawBitmap(Player.Ord.X, Player.Ord.Y, TankGfx, TANKGFX_WIDTH, TANKGFX_HEIGHT, WHITE);
  } else {
    if (Player.Ord.Status == EXPLODING) {
      for (i = 0; i < TANKGFX_WIDTH; i += 2) {
        display.drawBitmap(Player.Ord.X + i, Player.Ord.Y, TankGfx, random(4) + 2, 8, WHITE);
      }
      Player.ExplosionGfxCounter--;
      if (Player.ExplosionGfxCounter == 0) {
        Player.Ord.Status = DESTROYED;
        LoseLife();
      }
    }
  }
  
  if (Missile.Status == ACTIVE) {
    display.drawBitmap(Missile.X, Missile.Y, MissileGfx, MISSILE_WIDTH, MISSILE_HEIGHT, WHITE);
  }

  if (MotherShip.Ord.Status == ACTIVE) {
    display.drawBitmap(MotherShip.Ord.X, MotherShip.Ord.Y, MotherShipGfx, MOTHERSHIP_WIDTH, MOTHERSHIP_HEIGHT, WHITE);
  } else {
    if (MotherShip.Ord.Status == EXPLODING) {
      for (i = 0; i < MOTHERSHIP_WIDTH; i += 2) {
        display.drawBitmap(MotherShip.Ord.X + i, MotherShip.Ord.Y, ExplosionGfx, random(4) + 2, MOTHERSHIP_HEIGHT, WHITE);
      }
      MotherShip.ExplosionGfxCounter--;
      if (MotherShip.ExplosionGfxCounter == 0) {
        MotherShip.Ord.Status = DESTROYED;
      }
    }
  }
  
  display.display();
}

void LoseLife() {
  Player.Lives--;
  if (Player.Lives > 0) {
    DisplayPlayerAndLives(&Player);
    for (int i = 0; i < MAXBOMBS; i++) {
      AlienBomb[i].Status = DESTROYED;
      AlienBomb[i].Y = 0;
    }
    Player.Ord.Status = ACTIVE;
    Player.Ord.X = 0;
  } else {
    GameOver();
  }
}

void GameOver() {
  GameInPlay = false;
  display.clearDisplay();
  CentreText("Player 1", 0);
  CentreText("Game Over", 12);
  CentreText("Score ", 24);
  display.print(Player.Score);
  if (Player.Score > HiScore) {
    CentreText("NEW HIGH SCORE!!!", 36);
    CentreText("**CONGRATULATIONS**", 48);
  }
  display.display();
  if (Player.Score > HiScore) {
    HiScore = Player.Score;
    EEPROM.put(0, HiScore);
  }
  delay(2500);
}

void DisplayPlayerAndLives(PlayerStruct *Player) {
  display.clearDisplay();
  CentreText("Player 1", 0);
  CentreText("Score ", 12);
  display.print(Player->Score);
  CentreText("Lives ", 24);
  display.print(Player->Lives);
  CentreText("Level ", 36);
  display.print(Player->Level);
  display.display();
  delay(2000);
  Player->Ord.X = PLAYER_X_START;
}

void CentreText(const char *Text, unsigned char Y) {
  display.setCursor(int((float)(SCREEN_WIDTH)/2-((strlen(Text)*6)/2)), Y);
  display.print(Text);
}

void InitPlayer() {
  Player.Ord.Y = PLAYER_Y_START;
  Player.Ord.X = PLAYER_X_START;
  Player.Ord.Status = ACTIVE;
  Player.Lives = LIVES;
  Player.Level = 0;
  Player.Score = 0;
  Missile.Status = DESTROYED;
}

void NextLevel(PlayerStruct *Player) {
  int YStart;
  for (int i = 0; i < MAXBOMBS; i++) {
    AlienBomb[i].Status = DESTROYED;
  }
  AnimationFrame = false;
  Player->Level++;
  YStart = ((Player->Level-1) % LEVEL_TO_RESET_TO_START_HEIGHT) * AMOUNT_TO_DROP_BY_PER_LEVEL;
  InitAliens(YStart);
  AlienXMoveAmount = ALIEN_X_MOVE_AMOUNT;
  Player->AlienSpeed = INVADERS_SPEED;
  Player->AliensDestroyed = 0;
  MotherShip.Ord.X = -MOTHERSHIP_WIDTH;
  MotherShip.Ord.Status = DESTROYED;
  Missile.Status = DESTROYED;
  randomSeed(100);
  DisplayPlayerAndLives(Player);
}

void NewGame() {
  InitPlayer();
  NextLevel(&Player);
}

void InitAliens(int YStart) {
  for (int across = 0; across < NUM_ALIEN_COLUMNS; across++) {
    for (int down = 0; down < NUM_ALIEN_ROWS; down++) {
      Alien[across][down].Ord.X = X_START_OFFSET + (across * (LARGEST_ALIEN_WIDTH + SPACE_BETWEEN_ALIEN_COLUMNS)) - down;
      Alien[across][down].Ord.Y = YStart + (down * SPACE_BETWEEN_ROWS);
      Alien[across][down].Ord.Status=ACTIVE;
      Alien[across][down].ExplosionGfxCounter = EXPLOSION_GFX_TIME;
    }
  }
  MotherShip.Ord.Y = 0;
  MotherShip.Ord.X = -MOTHERSHIP_WIDTH;
  MotherShip.Ord.Status = DESTROYED;
}
