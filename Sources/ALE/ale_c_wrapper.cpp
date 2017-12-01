#include "ale_c_wrapper.h"

#include <cstring>
#include <string>
#include <stdexcept>

#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

//MARK: - Constants
// Define actions
enum Action {
    PLAYER_A_NOOP           = 0,
    PLAYER_A_FIRE           = 1,
    PLAYER_A_UP             = 2,
    PLAYER_A_RIGHT          = 3,
    PLAYER_A_LEFT           = 4,
    PLAYER_A_DOWN           = 5,
    PLAYER_A_UPRIGHT        = 6,
    PLAYER_A_UPLEFT         = 7,
    PLAYER_A_DOWNRIGHT      = 8,
    PLAYER_A_DOWNLEFT       = 9,
    PLAYER_A_UPFIRE         = 10,
    PLAYER_A_RIGHTFIRE      = 11,
    PLAYER_A_LEFTFIRE       = 12,
    PLAYER_A_DOWNFIRE       = 13,
    PLAYER_A_UPRIGHTFIRE    = 14,
    PLAYER_A_UPLEFTFIRE     = 15,
    PLAYER_A_DOWNRIGHTFIRE  = 16,
    PLAYER_A_DOWNLEFTFIRE   = 17,
    PLAYER_B_NOOP           = 18,
    PLAYER_B_FIRE           = 19,
    PLAYER_B_UP             = 20,
    PLAYER_B_RIGHT          = 21,
    PLAYER_B_LEFT           = 22,
    PLAYER_B_DOWN           = 23,
    PLAYER_B_UPRIGHT        = 24,
    PLAYER_B_UPLEFT         = 25,
    PLAYER_B_DOWNRIGHT      = 26,
    PLAYER_B_DOWNLEFT       = 27,
    PLAYER_B_UPFIRE         = 28,
    PLAYER_B_RIGHTFIRE      = 29,
    PLAYER_B_LEFTFIRE       = 30,
    PLAYER_B_DOWNFIRE       = 31,
    PLAYER_B_UPRIGHTFIRE    = 32,
    PLAYER_B_UPLEFTFIRE     = 33,
    PLAYER_B_DOWNRIGHTFIRE  = 34,
    PLAYER_B_DOWNLEFTFIRE   = 35,
    RESET                   = 40, // MGB: Use SYSTEM_RESET to reset the environment.
    UNDEFINED               = 41,
    RANDOM                  = 42,
    SAVE_STATE              = 43,
    LOAD_STATE              = 44,
    SYSTEM_RESET            = 45,
    LAST_ACTION_INDEX       = 50
};

std::string action_to_string(Action a);

//  Define datatypes
typedef std::vector<Action> ActionVect;

// reward type for RL interface
typedef int reward_t;


//MARK: - RAM
typedef unsigned char byte_t;

#define RAM_SIZE (128)

/** A simple wrapper around the Atari RAM. */
class ALERAM {
public:
    ALERAM();
    ALERAM(const ALERAM &rhs);
    
    ALERAM& operator=(const ALERAM &rhs);
    
    /** Byte accessors */
    byte_t get(unsigned int x) const;
    byte_t *byte(unsigned int x);
    
    /** Returns the whole array (equivalent to byte(0)). */
    byte_t *array() const { return (byte_t*)(m_ram); }
    
    size_t size() const { return sizeof(m_ram); }
    /** Returns whether two copies of the RAM are equal */
    bool equals(const ALERAM &rhs) const;
    
protected:
    byte_t m_ram[RAM_SIZE];
};

//MARK: - BSPF
// Types for 8-bit signed and unsigned integers
typedef signed char Int8;
typedef unsigned char uInt8;

// Types for 16-bit signed and unsigned integers
typedef signed short Int16;
typedef unsigned short uInt16;

// Types for 32-bit signed and unsigned integers
typedef signed int Int32;
typedef unsigned int uInt32;

//MARK: - Screen
typedef unsigned char pixel_t;

/** A simple wrapper around an Atari screen. */
class ALEScreen {
public:
    ALEScreen(int h, int w);
    ALEScreen(const ALEScreen &rhs);
    
    ALEScreen& operator=(const ALEScreen &rhs);
    
    /** pixel accessors, (row, column)-ordered */
    pixel_t get(int r, int c) const;
    pixel_t *pixel(int r, int c);
    
    /** Access a whole row */
    pixel_t *getRow(int r) const;
    
    /** Access the whole array */
    pixel_t *getArray() const;/* { return const_cast<pixel_t *>(&m_pixels[0]); }*/
    
    /** Dimensionality information */
    size_t height() const { return m_rows; }
    size_t width() const { return m_columns; }
    
    /** Returns the size of the underlying array */
    size_t arraySize() const { return m_rows * m_columns * sizeof(pixel_t); }
    
    /** Returns whether two screens are equal */
    bool equals(const ALEScreen &rhs) const;
    
protected:
    int m_rows;
    int m_columns;
    
    //std::vector<pixel_t> m_pixels;
};

//MARK: - Event

class Event
{
public:
    /**
     Enumeration of all possible events in Stella, including both
     console and controller event types as well as events that aren't
     technically part of the emulation core
     */
    enum Type
    {
        NoType,
        ConsoleOn, ConsoleOff, ConsoleColor, ConsoleBlackWhite,
        ConsoleLeftDifficultyA, ConsoleLeftDifficultyB,
        ConsoleRightDifficultyA, ConsoleRightDifficultyB,
        ConsoleSelect, ConsoleReset,
        
        JoystickZeroUp, JoystickZeroDown, JoystickZeroLeft,
        JoystickZeroRight, JoystickZeroFire,
        JoystickOneUp, JoystickOneDown, JoystickOneLeft,
        JoystickOneRight, JoystickOneFire,
        
        BoosterGripZeroTrigger, BoosterGripZeroBooster,
        BoosterGripOneTrigger, BoosterGripOneBooster,
        
        PaddleZeroResistance, PaddleZeroFire,
        PaddleZeroDecrease, PaddleZeroIncrease, PaddleZeroAnalog,
        PaddleOneResistance, PaddleOneFire,
        PaddleOneDecrease, PaddleOneIncrease, PaddleOneAnalog,
        PaddleTwoResistance, PaddleTwoFire,
        PaddleTwoDecrease, PaddleTwoIncrease, PaddleTwoAnalog,
        PaddleThreeResistance, PaddleThreeFire,
        PaddleThreeDecrease, PaddleThreeIncrease, PaddleThreeAnalog,
        
        KeyboardZero1, KeyboardZero2, KeyboardZero3,
        KeyboardZero4, KeyboardZero5, KeyboardZero6,
        KeyboardZero7, KeyboardZero8, KeyboardZero9,
        KeyboardZeroStar, KeyboardZero0, KeyboardZeroPound,
        
        KeyboardOne1, KeyboardOne2, KeyboardOne3,
        KeyboardOne4, KeyboardOne5, KeyboardOne6,
        KeyboardOne7, KeyboardOne8, KeyboardOne9,
        KeyboardOneStar, KeyboardOne0, KeyboardOnePound,
        
        DrivingZeroClockwise, DrivingZeroCounterClockwise, DrivingZeroValue,
        DrivingZeroFire,
        DrivingOneClockwise, DrivingOneCounterClockwise, DrivingOneValue,
        DrivingOneFire,
        
        ChangeState, LoadState, SaveState, TakeSnapshot, Quit,
        PauseMode, MenuMode, CmdMenuMode, DebuggerMode, LauncherMode,
        Fry, VolumeDecrease, VolumeIncrease,
        
        UIUp, UIDown, UILeft, UIRight, UIHome, UIEnd, UIPgUp, UIPgDown,
        UISelect, UINavPrev, UINavNext, UIOK, UICancel,
        
        LastType
    };
    
public:
    /**
     Create a new event object and use the given eventstreamer
     */
    Event();
    
    /**
     Destructor
     */
    virtual ~Event();
    
public:
    /**
     Get the value associated with the event of the specified type
     */
    virtual Int32 get(Type type) const;
    
    /**
     Set the value associated with the event of the specified type
     */
    virtual void set(Type type, Int32 value);
    
    /**
     Clears the event array (resets to initial state)
     */
    virtual void clear();
    
};


//MARK: - STATE
class ALEState {
public:
    ALEState();
    // Makes a copy of this state, also storing emulator information provided as a string
    ALEState(const ALEState &rhs, std::string serialized);
    
    // Restores a serialized ALEState
    ALEState(const std::string &serialized);
    
    /** Resets the system to its start state. numResetSteps 'RESET' actions are taken after the
     *  start. */
    void reset(int numResetSteps = 1);
    
    /** Returns true if the two states contain the same saved information */
    bool equals(ALEState &state);
    
    void resetPaddles(Event*);
    
    /** Applies paddle actions. This actually modifies the game state by updating the paddle
     *  resistances. */
    void applyActionPaddles(Event* event_obj, int player_a_action, int player_b_action);
    /** Sets the joystick events. No effect until the emulator is run forward. */
    void setActionJoysticks(Event* event_obj, int player_a_action, int player_b_action);
    
    void incrementFrame(int steps = 1);
    
    void resetEpisodeFrameNumber();
    
    //Get the frames executed so far
    int getFrameNumber() const;
    
    //Get the number of frames executed this episode.
    int getEpisodeFrameNumber() const;
    
    std::string serialize();
};

//MARK: - ColourPalette

class ColourPalette {
    
public:
    
    ColourPalette();
    
    /** Converts a given palette value in range [0, 255] into its RGB components. */
    void getRGB(int val, int &r, int &g, int &b) const;
    
    /** Converts a given palette value into packed RGB (format 0x00RRGGBB). */
    uInt32 getRGB(int val) const;
    
    /** Returns the byte-sized grayscale value for this palette index. */
    uInt8 getGrayscale(int val) const;
    
    /**
     Applies the current RGB palette to the src_buffer and returns the results in dst_buffer
     For each byte in src_buffer, three bytes are returned in dst_buffer
     8 bits => 24 bits
     */
    void applyPaletteRGB(uInt8* dst_buffer, uInt8 *src_buffer, size_t src_size);
    void applyPaletteRGB(std::vector<unsigned char>& dst_buffer, uInt8 *src_buffer, size_t src_size);
    
    /**
     Applies the current grayscale palette to the src_buffer and returns the results in dst_buffer
     For each byte in src_buffer, a single byte is returned in dst_buffer
     8 bits => 8 bits
     */
    void applyPaletteGrayscale(uInt8* dst_buffer, uInt8 *src_buffer, size_t src_size);
    void applyPaletteGrayscale(std::vector<unsigned char>& dst_buffer, uInt8 *src_buffer, size_t src_size);
    
    /**
     Loads all defined palettes with PAL color-loss data depending
     on 'state'.
     Sets the palette according to the given palette name.
     
     @param type  The palette type = {standard, z26, user}
     @param displayFormat The display format = { NTSC, PAL, SECAM }
     */
    void setPalette(const std::string& type,
                    const std::string& displayFormat);
    
    /**
     Loads a user-defined palette file (from OSystem::paletteFile), filling the
     appropriate user-defined palette arrays.
     */
    void loadUserPalette(const std::string& paletteFile);

};

//MARK: - ScreenExporter
class ScreenExporter {
    
public:
    
    /** Creates a new ScreenExporter which can be used to save screens using save(filename). */
    ScreenExporter(ColourPalette &palette);
    
    /** Creates a new ScreenExporter which will save frames successively in the directory provided.
     Frames are sequentially named with 6 digits, starting at 000000. */
    ScreenExporter(ColourPalette &palette, const std::string &path);
    
    /** Save the given screen to the given filename. No paths are created. */
    void save(const ALEScreen &screen, const std::string &filename) const;
    
    /** Save the given screen according to our own internal numbering. */
    void saveNext(const ALEScreen &screen);
};


//MARK: - ALEInterface
class ALEInterface {
public:
    ALEInterface();
    ~ALEInterface();
    // Legacy constructor
    ALEInterface(bool display_screen);
    
    // Get the value of a setting.
    std::string getString(const std::string& key);
    int getInt(const std::string& key);
    bool getBool(const std::string& key);
    float getFloat(const std::string& key);
    
    // Set the value of a setting. loadRom() must be called before the
    // setting will take effect.
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, const int value);
    void setBool(const std::string& key, const bool value);
    void setFloat(const std::string& key, const float value);
    
    // Resets the Atari and loads a game. After this call the game
    // should be ready to play. This is necessary after changing a
    // setting for the setting to take effect.
    void loadROM(std::string rom_file);
    
    // Applies an action to the game and returns the reward. It is the
    // user's responsibility to check if the game has ended and reset
    // when necessary - this method will keep pressing buttons on the
    // game over screen.
    reward_t act(Action action);
    
    // Indicates if the game has ended.
    bool game_over() const;
    
    // Resets the game, but not the full system.
    void reset_game();
    
    // Returns the vector of legal actions. This should be called only
    // after the rom is loaded.
    ActionVect getLegalActionSet();
    
    // Returns the vector of the minimal set of actions needed to play
    // the game.
    ActionVect getMinimalActionSet();
    
    // Returns the frame number since the loading of the ROM
    int getFrameNumber();
    
    // The remaining number of lives.
    int lives();
    
    // Returns the frame number since the start of the current episode
    int getEpisodeFrameNumber() const;
    
    // Returns the current game screen
    const ALEScreen &getScreen();
    
    //This method should receive an empty vector to fill it with
    //the grayscale colours
    void getScreenGrayscale(std::vector<unsigned char>& grayscale_output_buffer);
    
    //This method should receive a vector to fill it with
    //the RGB colours. The first positions contain the red colours,
    //followed by the green colours and then the blue colours
    void getScreenRGB(std::vector<unsigned char>& output_rgb_buffer);
    
    // Returns the current RAM content
    const ALERAM &getRAM();
    
    // Saves the state of the system
    void saveState();
    
    // Loads the state of the system
    void loadState();
    
    // This makes a copy of the environment state. This copy does *not* include pseudorandomness,
    // making it suitable for planning purposes. By contrast, see cloneSystemState.
    ALEState cloneState();
    
    // Reverse operation of cloneState(). This does not restore pseudorandomness, so that repeated
    // calls to restoreState() in the stochastic controls setting will not lead to the same outcomes.
    // By contrast, see restoreSystemState.
    void restoreState(const ALEState& state);
    
    // This makes a copy of the system & environment state, suitable for serialization. This includes
    // pseudorandomness and so is *not* suitable for planning purposes.
    ALEState cloneSystemState();
    
    // Reverse operation of cloneSystemState.
    void restoreSystemState(const ALEState& state);
    
    // Save the current screen as a png file
    void saveScreenPNG(const std::string& filename);
    
    // Creates a ScreenExporter object which can be used to save a sequence of frames. Ownership
    // said object is passed to the caller. Frames are saved in the directory 'path', which needs
    // to exists.
    ScreenExporter *createScreenExporter(const std::string &path) const;
};



ALEInterface * interface(const void *object) {
    ALEInterface *ale;
    ale = (ALEInterface *)object;
    return ale;
}

const void * initializeInterface() {
    ALEInterface *ale = new ALEInterface();
    return (void *)ale;
}

void deleteInterface(const void *object) {
    delete interface(object);
}

int act(const void *object, int action) {
    return interface(object)->act((Action) action);
}

void getLegalActionSet(const void *object, int *actions, size_t *action_count) {
    ALEInterface *ale = interface(object);
    
    ActionVect legal_actions = ale->getLegalActionSet();
    size_t size = legal_actions.size();

    for (int i = 0; i<size; i++) {
        int action = (int) legal_actions[i];
        actions[i] = action;
    }
    *action_count = size;
}

void reset_game(const void *object) {
    ALEInterface *ale = interface(object);
    ale->reset_game();
}

int getScreenWidth(const void *object) {
    ALEInterface *ale = interface(object);
    return  ale->getScreen().width();
}

int getScreenHeight(const void *object) {
    ALEInterface *ale = interface(object);
    return  ale->getScreen().height();
}
void getScreenRGB(const void *object, unsigned char *output_buffer){
    ALEInterface *ale = interface(object);
    std::vector<unsigned char> rgb_buffer;

    ale->getScreenRGB(rgb_buffer);
    size_t size = rgb_buffer.size();
    output_buffer = rgb_buffer.data();
}

char game_over(const void *object) {
    return (char) interface(object)->game_over();
}

void saveScreenPNG(const void *object, const char *file_path) {
    ALEInterface *ale = interface(object);
    ale->saveScreenPNG(file_path);
}

void loadROM(const void *object, const char *rom_file) {
    ALEInterface *ale;
    ale = (ALEInterface *)object;
    ale->setInt("random_seed", 0);
    ale->loadROM(rom_file);
}


void encodeState(ALEState *state, char *buf, int buf_len) {
	std::string str = state->serialize();

	if (buf_len < int(str.length())) {
		throw new std::runtime_error("Buffer is not big enough to hold serialized ALEState. Please use encodeStateLen to determine the correct buffer size");
	}
	memcpy(buf, str.data(), str.length());
}

int encodeStateLen(ALEState *state) {
	return state->serialize().length();
}

ALEState *decodeState(const char *serialized, int len) {
	std::string str(serialized, len);

	return new ALEState(str);
}

