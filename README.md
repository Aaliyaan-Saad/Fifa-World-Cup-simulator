# World Cup Simulator

A C++ console-based World Cup simulator that recreates a 32-team FIFA World Cup-style tournament. The project includes a full group stage, automatic table ranking, qualification of the top two teams from each group, and knockout rounds until a champion is crowned.

The simulator uses team ratings, form multipliers, probability-based goal generation, and penalty shootouts to make each tournament run feel different.

---

## Features

- Simulates the 2022 FIFA World Cup group structure
- Includes 32 teams divided into 8 groups
- Each team has separate ratings for:
  - Attack
  - Midfield
  - Defense
- Simulates all group-stage matches
- Automatically updates:
  - Points
  - Goals scored
  - Goals conceded
  - Goal difference
  - Wins, draws, and losses
  - Form multiplier
- Sorts group tables using:
  1. Points
  2. Goal difference
  3. Goals scored
- Selects the top two teams from each group
- Simulates knockout rounds using real World Cup-style pairings
- Includes penalty shootouts if knockout matches end in a draw
- Displays the final World Cup champion
- Includes ASCII art title screen

---

## How the Simulation Works

### Team Strength

Each team has an overall strength calculated using attack, midfield, and defense ratings:

```cpp
overallStrength = (attack * 0.33 + midfield * 0.34 + defense * 0.33) * formMultiplier;
```

Midfield has a slightly higher weight because it connects both attack and defense.

### Form Multiplier

Teams gain or lose form depending on match results:

- Win: form increases by `0.02`
- Loss: form decreases by `0.02`
- Draw: form stays mostly stable

The form multiplier is capped between:

```cpp
0.90 and 1.10
```

This means teams can improve or decline during the tournament, but not unrealistically.

### Match Probability

The simulator calculates each team's chance of winning based on their effective strength. The strengths are squared to reduce the number of unrealistic upsets:

```cpp
weightedStrength = effectiveStrength * effectiveStrength;
```

This gives stronger teams a better chance while still allowing weaker teams to pull off surprises.

### Goal Generation

Goals are generated using a probability system. Each team gets a fixed number of scoring chances, and each chance may become a goal depending on the team’s calculated probability.

---

## Project Structure

The project is written in a single C++ file.

```text
ucl.cpp
```

Main components:

| Component | Purpose |
|---|---|
| `Team` class | Stores team ratings, stats, form, and match history |
| `Group` class | Handles group-stage matches, tables, and qualification |
| `Knockout` class | Handles knockout rounds, finals, and penalties |
| `playMatch()` | Simulates a normal group-stage match |
| `calculateTeamProbability()` | Calculates win/scoring probability based on team strength |
| `generateGoals()` | Generates goals using randomness and probability |
| `main()` | Creates teams, groups, user prompts, and runs the full tournament |

---

## How to Run

### 1. Compile the program

Using g++:

```bash
g++ ucl.cpp -o worldcup
```

### 2. Run the program

On Windows:

```bash
worldcup.exe
```

On macOS/Linux:

```bash
./worldcup
```

---

## User Flow

When the program starts, it displays the World Cup Simulator ASCII title.

The user is then asked to:

1. Press `y` to start the simulation
2. Press `y` to display group tables
3. Press `y` to show qualified teams
4. Press `y` to simulate the knockout rounds

If the user enters anything other than `y`, the simulation ends.

---

## Example Output

```text
--- Matches in Group A ---
Qatar 0 - 2 Ecuador
Qatar 1 - 3 Senegal
Qatar 0 - 4 Netherlands
Ecuador 1 - 1 Senegal
Ecuador 0 - 2 Netherlands
Senegal 1 - 2 Netherlands
```

Example final output:

```text
============================
Champion: Argentina
============================
```

Because the simulator uses randomness, every run can produce different results.

---

## Teams Included

The simulator includes the 32 teams from the 2022 FIFA World Cup:

- Qatar
- Ecuador
- Senegal
- Netherlands
- England
- Iran
- USA
- Wales
- Argentina
- Saudi Arabia
- Mexico
- Poland
- France
- Australia
- Denmark
- Tunisia
- Spain
- Costa Rica
- Germany
- Japan
- Belgium
- Canada
- Morocco
- Croatia
- Brazil
- Serbia
- Switzerland
- Cameroon
- Portugal
- Ghana
- Uruguay
- South Korea

---

## Possible Future Improvements

- Add extra time before penalty shootouts
- Improve penalty shootout realism
- Add player-level ratings instead of only team-level ratings
- Add injuries, red cards, and suspensions
- Add home advantage or crowd effect
- Store results in a file
- Add a menu system instead of repeated `y` prompts
- Allow the user to create custom teams
- Add a full 48-team World Cup format
- Improve goal generation using attack vs defense matchups
- Add match statistics such as possession, shots, and expected goals

---

## What I Learned

This project helped practice important C++ and object-oriented programming concepts, including:

- Classes and objects
- Constructors
- Encapsulation
- Vectors
- Sorting with custom conditions
- Passing objects by reference and value
- Random number generation
- Probability-based simulation
- Separating logic into different classes
- Building a complete tournament system from smaller components

---

## Author

Created as a C++ object-oriented programming project.
