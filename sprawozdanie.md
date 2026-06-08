# Sprawozdanie z projektu: Zomwave 2D

**Autor:** [Imię i Nazwisko]  
**Przedmiot:** Programowanie w języku C++  
**Semestr:** II  
**Rok akademicki:** 2025/2026

---

## 1. Wprowadzenie

**Zomwave 2D** to dwuwymiarowa gra komputerowa typu "top-down zombie survival" stworzona w języku C++20 z wykorzystaniem biblioteki graficznej **raylib**. Gracz wciela się w postać, która musi przetrwać kolejne fale przeciwników (zombie, szkielety, bossowie), zdobywać monety, ulepszać broń oraz statystyki postaci.

Projekt został zrealizowany jako zaliczenie przedmiotu "Programowanie w języku C++" na drugim semestrze studiów inżynierskich. Gra została zaprojektowana z myślą o płynnej rozgrywce, czytelnej architekturze kodu oraz możliwości łatwej modyfikacji balansu poprzez zewnętrzne pliki konfiguracyjne.

### 1.1. Cel projektu

Celem projektu było stworzenie w pełni funkcjonalnej gry 2D z:

- Płynnym systemem animacji klatkowych
- Zaawansowanym systemem broni z możliwością ulepszania
- Sztuczną inteligencją przeciwników
- Systemem fal o rosnącym poziomie trudności
- Dynamicznym oświetleniem
- Interfejsem użytkownika (HUD, menu, sklep, ustawienia)
- Obsługą platformy desktopowej i webowej

---

## 2. Użyte technologie i biblioteki

| Technologia               | Zastosowanie                                     |
| ------------------------- | ------------------------------------------------ |
| **C++20**                 | Język programowania                              |
| **raylib 5.x**            | Silnik graficzny (renderowanie, dźwięk, wejście) |
| **raygui**                | GUI (przyciski, suwaki)                          |
| **LDtkLoader**            | Wczytywanie map w formacie LDtk                  |
| **nlohmann/json v3.11.3** | Parsowanie plików konfiguracyjnych JSON          |
| **fmtlib**                | Formatowanie tekstu                              |
| **Box2D**                 | Fizyka (zadeklarowana w CMake, opcjonalna)       |
| **CMake 3.22+**           | System budowania                                 |
| **Emscripten**            | Kompilacja na platformę Web                      |
| **Doxygen**               | Generowanie dokumentacji kodu                    |

---

## 3. Opis wykonanych zadań

### 3.1. Implementacja głównej pętli gry

Stworzono klasę `Game`, która zarządza stanami gry (menu główne, gra, pauza, ustawienia, koniec gry) i koordynuje wszystkie podsystemy. Zaimplementowano obsługę dwóch platform: desktopowej (pętla `while`) i webowej (`emscripten_set_main_loop_arg`).

### 3.2. System animacji

Zaimplementowano klasę `Animator` obsługującą animacje klatkowe z możliwością definiowania wielu stanów (np. "idle", "walk", "shoot", "reload"). Każda animacja posiada własne FPS, zestaw klatek oraz flagę zapętlenia.

### 3.3. System broni

Stworzono strukturę `Weapon` oraz cztery typy broni: Glock, MP5, AK-47 i Shotgun. Każda broń posiada unikalne parametry (obrażenia, amunicja, szybkostrzelność, rozrzut) oraz własne animacje strzału i przeładowania. Broń można ulepszać w sklepie.

### 3.4. System przeciwników

Zaimplementowano hierarchię klas: abstrakcyjną klasę bazową `Enemy` oraz trzy klasy pochodne:

- **Zombie** – wróg melee, podąża za graczem
- **Skeleton** – wróg dystansowy, strzela z łuku
- **Boss** – zaawansowany wróg z wieloma animacjami i atakiem dystansowym

### 3.5. System fal

Zaimplementowano `WaveManager`, który generuje fale przeciwników proceduralnie. Z każdą falą zwiększa się liczba i różnorodność przeciwników, a odstępy między spawnami maleją.

### 3.6. System pocisków

Stworzono `BulletManager` obsługujący strzelanie, aktualizację pozycji pocisków, kolizję ze ścianami oraz automatyczne usuwanie po upływie czasu życia.

### 3.7. System oświetlenia

Zaimplementowano dynamiczne oświetlenie poprzez renderowanie maski światła na osobnym RenderTexture z wykorzystaniem trybu mieszania `BLEND_ADDITIVE` i `BLEND_MULTIPLIED`.

### 3.8. Interfejs użytkownika

Stworzono:

- Menu główne z przyciskami (GRAJ, USTAWIENIA, WYJDŹ)
- Ekran pauzy
- Ekran ustawień (głośność muzyki, SFX, limit FPS)
- Ekran końca gry z informacją o osiągniętej fali
- HUD podczas gry (pasek zdrowia, amunicja, monety, fala, timer)
- Sklep z ulepszeniami

### 3.9. Konfiguracja gry

Wszystkie parametry balansu gry (statystyki gracza, broni, przeciwników, fal) są przechowywane w pliku `config.json` i wczytywane przez `ConfigManager` z użyciem biblioteki nlohmann/json.

### 3.10. Dokumentacja kodu

Cały kod źródłowy został opatrzony komentarzami Doxygen, umożliwiającymi automatyczne generowanie dokumentacji HTML.

---

## 4. Możliwości projektu

- **Płynne animacje** – system animacji klatkowych z przełączaniem stanów
- **4 rodzaje broni** – Glock, MP5, AK-47, Shotgun – każda z unikalnymi parametrami
- **Ulepszanie broni** – każda broń posiada wiele poziomów ulepszeń
- **3 typy przeciwników** – Zombie (melee), Skeleton (dystansowy), Boss (zaawansowany)
- **Proceduralne fale** – rosnący poziom trudności z każdą falą
- **Dynamiczne oświetlenie** – maska światła wokół gracza i pocisków
- **System monet** – zbieranie monet z magnesem przyciągającym
- **System bomb** – rzucanie bomb z efektem wybuchu
- **Sklep** – ulepszanie zdrowia, szybkości, zasięgu widzenia, zakup mikstur i bomb
- **Zapis stanu** – zapisywanie high score i ustawień do plików JSON
- **Wirtualna rozdzielczość** – skalowanie gry do dowolnego rozmiaru okna
- **Obsługa Web** – kompilacja na platformę przeglądarkową przez Emscripten
- **Konfigurowalny balans** – wszystkie parametry w pliku JSON
- **Dźwięk przestrzenny** – tłumienie dźwięku w zależności od odległości

---

## 5. Skróty klawiszowe

| Klawisz                 | Akcja                    |
| ----------------------- | ------------------------ |
| **W / A / S / D**       | Ruch postaci             |
| **Lewy przycisk myszy** | Strzelanie (przytrzymać) |
| **R**                   | Przeładowanie broni      |
| **E**                   | Użycie mikstury zdrowia  |
| **B**                   | Otwarcie sklepu          |
| **G**                   | Rzut bombą               |
| **ESC**                 | Pauza / wznowienie       |

---

## 6. Działanie gry

### 6.1. Przebieg rozgrywki

1. Gracz rozpoczyna w menu głównym, gdzie widzi swój najlepszy wynik (high score).
2. Po kliknięciu "GRAJ" rozpoczyna się pierwsza fala przeciwników.
3. Przeciwnicy pojawiają się w losowych miejscach na mapie i atakują gracza.
4. Gracz może strzelać, przeładowywać, używać mikstur i bomb.
5. Po pokonaniu przeciwników wypadają monety, które gracz zbiera.
6. Co 60 sekund rozpoczyna się nowa, trudniejsza fala.
7. W przerwach gracz może otworzyć sklep (klawisz B) i ulepszyć swoją postać.
8. Po śmierci gracza wyświetlany jest ekran końca gry z osiągniętą falą.

### 6.2. System walki

Gracz celuje myszą, a broń automatycznie obraca się w kierunku kursora. Strzelanie odbywa się przez przytrzymanie lewego przycisku myszy. Każda broń ma inny czas przeładowania, rozrzut i liczbę pocisków na strzał. Shotgun strzela kilkoma pociskami jednocześnie z dużym rozrzutem.

### 6.3. System ulepszeń

W sklepie gracz może:

- Zwiększyć maksymalne zdrowie (6 poziomów)
- Zwiększyć szybkość poruszania (5 poziomów)
- Zwiększyć zasięg widzenia (5 poziomów)
- Ulepszyć aktualnie trzymaną broń
- Kupić mikstury zdrowia
- Kupić bomby

### 6.4. Sztuczna inteligencja przeciwników

- **Zombie** – podąża bezpośrednio za graczem, atakuje z bliska
- **Skeleton** – utrzymuje dystans, strzela z łuku, ucieka gdy gracz podejdzie zbyt blisko
- **Boss** – podąża za graczem, w zasięgu ataku zatrzymuje się i strzela butelkami z trucizną, okresowo pije miksturę (efekt dźwiękowy)

---

## 7. Najważniejsze fragmenty kodu

### 7.1. Główna pętla gry

Pętla gry wykorzystuje wzorzec **stanu** (`GameState`) do przełączania między menu, grą, pauzą i ekranem końca gry. Na platformie Web używana jest funkcja `emscripten_set_main_loop_arg`, na desktopie zwykła pętla `while`.

```cpp
void Game::Run() {
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop_arg(MainLoopHelper, this, 0, 1);
#else
    while (!WindowShouldClose() && currentState != GameState::EXIT) {
        MainLoopHelper(this);
    }
#endif
}
```

Renderowanie odbywa się w **wirtualnej rozdzielczości** 800×450, która jest skalowana do rzeczywistego rozmiaru okna, co zapewnia spójny wygląd na różnych ekranach:

```cpp
float scale = fminf((float)GetScreenWidth() / virtualWidth,
                    (float)GetScreenHeight() / virtualHeight);
Rectangle dest = {(GetScreenWidth() - (virtualWidth * scale)) * 0.5f,
                  (GetScreenHeight() - (virtualHeight * scale)) * 0.5f,
                  virtualWidth * scale, virtualHeight * scale};
DrawTexturePro(target.texture, source, dest, {0, 0}, 0.0f, WHITE);
```

### 7.2. System animacji

Klasa `Animator` zarządza animacjami klatkowymi. Obsługuje wiele stanów (np. "idle", "walk", "shoot") i przełączanie między nimi:

```cpp
struct Animation {
    Texture2D *texture;              // Wskaźnik do tekstury
    Vector2 frameSize;               // Rozmiar pojedynczej klatki
    std::vector<Vector2> frames;     // Pozycje klatek na spritesheecie
    float fps;                       // Klatki na sekundę
    bool isLooping;                  // Czy animacja zapętla się
};

class Animator {
    void Update(float dt);           // Aktualizacja timera klatek
    void Draw(Rectangle position, bool flipX, float rotation);
    void SetState(std::string state); // Zmiana stanu animacji
    void AddAnimation(std::string state, Texture2D *texture,
                      Vector2 frameSize, float fps,
                      std::vector<Vector2> framesPos, bool loop);
};
```

### 7.3. System broni i strzelania

Każda broń przechowuje swoje statystyki i ładuje je z pliku konfiguracyjnego. Strzelanie uwzględnia pozycję lufy, obrót broni w kierunku myszy oraz rozrzut:

```cpp
struct Weapon {
    std::string name;
    WeaponType type;
    int currentLevel;
    float damage;
    float fireCooldown;
    int pellets;
    float spread;
    int currentAmmo;
    int maxAmmo;
    WeaponState state;
    Animator animator;

    void LoadStatsFromBalance(const GameBalance &balance);
};

void Player::UpdateWeapon(float dt, Vector2 mousePos, BulletManager *bulletManager) {
    CalculateWeaponPos(mousePos);
    // ...
    for (int i = 0; i < wp->pellets; ++i) {
        float randomOffset = GetRandomValue(-wp->spread * 10, wp->spread * 10) / 10.0f;
        float finalAngleRad = angleRad + (randomOffset * DEG2RAD);
        Vector2 finalDirection = {cosf(finalAngleRad), sinf(finalAngleRad)};
        Vector2 bulletTarget = Vector2Add(barrelEnd, Vector2Scale(finalDirection, 1000));
        bulletManager->Shoot(barrelEnd, bulletTarget, wp->damage,
                             BulletType::BULLET, 800, randomOffset, false);
    }
}
```

### 7.4. System przeciwników – klasa bazowa

Hierarchia klas z abstrakcyjną klasą bazową `Enemy`:

```cpp
class Enemy {
public:
    Vector2 position;
    bool active = true;
    float health, maxHealth;
    float speed, damage;
    int killReward, dropReward;

    virtual void Update(float dt, Vector2 playerPos, Map *map) = 0;
    virtual void Draw() = 0;
    virtual Rectangle GetHitbox() = 0;
    virtual void TakeDamage(float damage);
    virtual void DrawHealthBar();
};
```

### 7.5. Implementacja Zombie – wróg melee

Zombie podąża za graczem, z osobna sprawdzając kolizję dla osi X i Y, co zapobiega blokowaniu się na ścianach:

```cpp
void Zombie::Update(float dt, Vector2 playerPos, Map *map) {
    Vector2 dir = Vector2Normalize(Vector2Subtract(playerPos, position));
    float moveX = dir.x * speed * dt;
    float moveY = dir.y * speed * dt;

    if (moveX != 0.0f) {
        Rectangle futureHitboxX = {hitX + moveX, hitY, width, height};
        if (map->CheckHitbox(futureHitboxX) == false)
            position.x += moveX;
    }
    if (moveY != 0.0f) {
        Rectangle futureHitboxY = {hitX, hitY + moveY, width, height};
        if (map->CheckHitbox(futureHitboxY) == false)
            position.y += moveY;
    }
}
```

### 7.6. Implementacja Boss – zaawansowany przeciwnik

Boss posiada trzy animacje (idle, walk, attack) i atak dystansowy. Używa maszyny stanów do przełączania między zachowaniami:

```cpp
void Boss::Update(float dt, Vector2 playerPos, Map *map) {
    // ...
    if (distance > stopRange && canMove) {
        animator.SetState("walk");
        // poruszanie się w kierunku gracza
    }
    if (distance <= attackRange && attackTimer <= 0.0f) {
        animator.SetState("attack");
        // wystrzelenie pocisku w momencie trafienia klatki ataku
        bulletManager->Shoot(arrowStart, arrowTarget, damage,
                             BulletType::BOTTLE, 350.0f, 0, true);
    }
}
```

### 7.7. System fal

Fale są generowane proceduralnie – im wyższa fala, tym więcej i silniejszych przeciwników. Odstęp między spawnami maleje z każdą falą:

```cpp
void WaveManager::Update(float dt, EnemyManager *enemyManager,
                         Vector2 playerPos, Map *map, const GameBalance &gb) {
    globalTime += dt;
    waveTimer += dt;

    if (waveTimer >= waveDuration) {
        currentWave++;
        waveTimer = 0.0f;
    }

    float currentSpawnInterval = gb.waves.baseSpawnInterval /
        (1.0f + (currentWave * gb.waves.waveMultiplier));

    if (currentSpawnInterval < gb.waves.minSpawnInterval)
        currentSpawnInterval = gb.waves.minSpawnInterval;

    // Losowanie typu przeciwnika z rosnącą trudnością
    int skeletonChance = 10 + (currentWave * 5);  // max 40%
    int bossChance = (currentWave >= 5) ? 2 + ((currentWave - 5) * 2) : 0; // max 15%
    // ...
}
```

### 7.8. System pocisków

Manager pocisków obsługuje strzelanie, aktualizację pozycji, kolizję ze ścianami oraz automatyczne usuwanie po upływie czasu życia:

```cpp
void BulletManager::Shoot(Vector2 startPos, Vector2 targetPos, float damage,
                          BulletType bulletType, float speed, float spreadAngle,
                          bool isEnemy) {
    Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, startPos));
    float angle = atan2(direction.y, direction.x) + spreadAngle * DEG2RAD;
    Vector2 finalDirection = {cos(angle), sin(angle)};
    Vector2 velocity = Vector2Scale(finalDirection, speed);
    // ...
}

void BulletManager::Update(float dt, Map *map) {
    for (auto &b : bullets) {
        b.position = Vector2Add(b.position, Vector2Scale(b.velocity, dt));
        b.lifeTimer -= dt;
        if (b.lifeTimer <= 0.0f || map->CheckHitbox(b.GetHitbox()))
            b.active = false;
    }
    std::erase_if(bullets, [](const Bullet &b) { return !b.active; });
}
```

### 7.9. System oświetlenia

Dynamiczne oświetlenie realizowane jest poprzez renderowanie maski światła na osobnym RenderTexture, a następnie nałożenie jej w trybie `BLEND_MULTIPLIED`:

```cpp
void Game::DrawLights() {
    BeginTextureMode(lightMask);
    ClearBackground(Color{0, 0, 0, 255});  // Ciemność
    BeginBlendMode(BLEND_ADDITIVE);
    BeginMode2D(camera);

    // Światło wokół gracza
    DrawCircleGradient(player->GetPosition(), player->visionRadius,
                       Color{255, 255, 255, 255}, BLANK);

    // Światła wokół pocisków
    for (const auto &bullet : bulletManager->bullets) {
        if (bullet.active) {
            Color bulletColor = bullet.isEnemy
                ? Color{0, 200, 0, 120}    // wrogie – zielone
                : Color{255, 200, 0, 150}; // przyjazne – żółte
            DrawCircleGradient(bullet.position, 60, bulletColor, BLANK);
        }
    }
    EndMode2D();
    EndBlendMode();
    EndTextureMode();
}
```

### 7.10. Konfiguracja gry (config.json)

Wszystkie parametry balansu gry są przechowywane w pliku `config.json` i wczytywane przez `ConfigManager` za pomocą biblioteki **nlohmann/json**:

```cpp
static GameBalance LoadBalance(const std::string& filepath = "config.json") {
    GameBalance balance;
    std::ifstream file(filepath);
    if(file.is_open()) {
        try {
            json j;
            file >> j;
            balance = j.get<GameBalance>();
        } catch(const std::exception& e) {
            printf("ERROR: %s\n", e.what());
        }
    }
    return balance;
}
```

Przykładowa struktura pliku JSON:

```json
{
    "player": {
        "coins": 100000,
        "potions": 3,
        "bombs": 1,
        "maxHealthLevels": [100, 130, 160, 190, 220, 240],
        "speedLevels": [220, 240, 260, 280, 300]
    },
    "weapons": {
        "Glock": {
            "unlockCost": 0,
            "damage": [15, 17, 19, 21, 24],
            "maxAmmo": [12, 14, 16, 18, 18],
            "fireCooldown": [0.45, 0.40, 0.35, 0.28, 0.22]
        }
    },
    "enemies": {
        "Zombie": { "health": 100, "speed": 120, "damage": 10, "killReward": 2 },
        "Boss":   { "health": 500, "speed": 60,  "damage": 48, "killReward": 350 }
    }
}
```

### 7.11. Odpychanie przeciwników

W `EnemyManager::Update` zaimplementowano odpychanie przeciwników od siebie, aby nie nachodzili na siebie nawzajem:

```cpp
for (size_t i = 0; i < enemies.size(); i++) {
    for (size_t j = i + 1; j < enemies.size(); j++) {
        Enemy *e1 = enemies[i].get();
        Enemy *e2 = enemies[j].get();
        Vector2 diff = Vector2Subtract(e1->position, e2->position);
        float distance = Vector2Length(diff);
        float minDistance = std::max(e1->GetHitbox().height, e2->GetHitbox().height);
        if (distance < minDistance && distance > 0.01f) {
            float overlap = minDistance - distance;
            Vector2 direction = Vector2Normalize(diff);
            Vector2 pushVector = Vector2Scale(direction, overlap * 0.5f);
            e1->position = Vector2Add(e1->position, pushVector);
            e2->position = Vector2Subtract(e2->position, pushVector);
        }
    }
}
```

---

## 8. Screeny

_(Miejsce na zrzuty ekranu – do wstawienia przez autora)_

- **Rysunek 1:** Menu główne gry
- **Rysunek 2:** Rozgrywka – walka z przeciwnikami
- **Rysunek 3:** Ekran sklepu z ulepszeniami
- **Rysunek 4:** Ekran ustawień
- **Rysunek 5:** Ekran końca gry

---

## 9. Podsumowanie i wnioski

Projekt **Zomwave 2D** został zrealizowany zgodnie z założeniami, dostarczając w pełni funkcjonalną grę typu zombie survival. W trakcie realizacji projektu:

- **Zastosowano** nowoczesny standard C++20 oraz sprawdzone biblioteki (raylib, nlohmann/json)
- **Zaimplementowano** czytelną architekturę opartą na wzorcu Manager + Entity
- **Zapewniono** konfigurowalność balansu gry bez konieczności ponownej kompilacji
- **Udokumentowano** cały kod źródłowy komentarzami Doxygen
- **Zapewniono** wsparcie dla dwóch platform: desktopowej i webowej

Gra oferuje płynną rozgrywkę, dynamiczne oświetlenie, zaawansowany system broni oraz proceduralnie generowane fale przeciwników z rosnącym poziomem trudności. Kod źródłowy jest w pełni udokumentowany za pomocą komentarzy Doxygen, a dokumentacja HTML może być wygenerowana poleceniem CMake.

---

## 10. Bibliografia

1. Raylib – https://www.raylib.com/
2. LDtk – https://ldtk.io/
3. nlohmann/json – https://github.com/nlohmann/json
4. Emscripten – https://emscripten.org/
5. Doxygen – https://www.doxygen.nl/
6. Raygui – https://github.com/raysan5/raygui
7. fmtlib – https://fmt.dev/

