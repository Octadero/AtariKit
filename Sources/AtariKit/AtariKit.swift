import CALE
import Foundation

typealias ALEInterface = OpaquePointer

public enum EnvironmentError: Error {
    case romNotFound
}

public enum EnvironmentAction: Int32 {
    case playerAnoop            = 0
    case playerAfire            = 1
    case playerAup              = 2
    case playerAright           = 3
    case playerAleft            = 4
    case playerAdown            = 5
    case playerAupright         = 6
    case playerAupleft          = 7
    case playerAdownright       = 8
    case playerAdownleft        = 9
    case playerAupfire          = 10
    case playerArightfire       = 11
    case playerAleftfire        = 12
    case playerAdownfire        = 13
    case playerAuprightfire     = 14
    case playerAupleftfire      = 15
    case playerAdownrightfire   = 16
    case playerAdownleftfire    = 17
    case playerBnoop            = 18
    case playerBfire            = 19
    case playerBup              = 20
    case playerBright           = 21
    case playerBleft            = 22
    case playerBdown            = 23
    case playerBupright         = 24
    case playerBupleft          = 25
    case playerBdownright       = 26
    case playerBdownleft        = 27
    case playerBupfire          = 28
    case playerBrightfire       = 29
    case playerBleftfire        = 30
    case playerBdownfire        = 31
    case playerBuprightfire     = 32
    case playerBupleftfire      = 33
    case playerBdownrightfire   = 34
    case playerBdownleftfire    = 35
    case reset                  = 40// MGB: Use SYSTEM_RESET to reset the environment.
    case UNDEFINED              = 41
    case random                 = 42
    case saveState              = 43
    case loadState              = 44
    case systemReset            = 45
    case lastActionIndex        = 50
    case allValues
};

/// Represent Atari game environment.
public class Environment {
    private let aleInterface: ALEInterface?
    private var bufferPointer: UnsafeMutablePointer<UInt8> = UnsafeMutablePointer<UInt8>.allocate(capacity: 0)
    private var pixelsCount: Int = 0
    
    /// Load `Environment` from rom with game.
    public init(romPath: String) throws {
        guard FileManager.default.fileExists(atPath: romPath) else {
            throw EnvironmentError.romNotFound
        }
        
        aleInterface = CALE.ALE_new()
        
        romPath.withCString { (pointer)  in
            CALE.loadROM(aleInterface, pointer)
        }
        
        let count = Int(CALE.getScreenWidth(aleInterface) * CALE.getScreenHeight(aleInterface)) * 3
        pixelsCount = count
        bufferPointer = UnsafeMutablePointer<UInt8>.allocate(capacity: count)
        bufferPointer.initialize(to: 0, count: count)
    }
    
    /// Do `EnvironmentAction` and return reward.
    public func send(action: EnvironmentAction) -> Int {
        return Int(CALE.act(aleInterface, action.rawValue))
    }
    
    /// Return array of available `EnvironmentAction` for loaded rom.
    public func legalActions() -> [EnvironmentAction] {
        let actions = UnsafeMutablePointer<Int32>.allocate(capacity: Int(EnvironmentAction.allValues.rawValue))
        let count = UnsafeMutablePointer<Int32>.allocate(capacity: 1)
        
        CALE.getLegalActionSet(aleInterface, actions)
        
        let bufferCount: Int32 = count.pointee
        let buffer = UnsafeBufferPointer(start: actions, count: Int(bufferCount))
        let arrayOfActions = Array(buffer)
        actions.deinitialize()
        count.deinitialize()
        
        return arrayOfActions.flatMap { EnvironmentAction(rawValue: $0) }
    }
    
    /// Reset game in `Environment`
    public func reset() {
        CALE.reset_game(aleInterface)
    }
    
    /// Return screen size width and height
    public func screenSize() -> (width: Int, height: Int) {
        return (Int(CALE.getScreenWidth(aleInterface)) , Int(CALE.getScreenHeight(aleInterface)))
    }
    
    /// Draw and return RGB screen buffer
    public func screenRGBBuffer() -> Array<UInt8> {
        getScreenRGB(aleInterface, bufferPointer)
        // width * height * R G B components
        let buffer = UnsafeBufferPointer(start: bufferPointer, count: pixelsCount)
        let rgbBites = Array(buffer)
        return rgbBites
    }
    
    /// True when game is finished, after that call reset to continue. 
    public var isOver: Bool {
        let over: UInt8 = CALE.game_over(aleInterface)
        return over != 0
    }
    
    /// Save current screen as png image with passed path
    public func saveScreen(at path: String) {
        path.withCString { (pathPointer) in
            CALE.saveScreenPNG(aleInterface, pathPointer)
        }
    }
        
    deinit {
        bufferPointer.deinitialize(count: pixelsCount)
        //  Leeds to error free(): invalid next size (fast): 0x0000000002252f20 ***
        //  deleteCInterface(aleInterface)
    }
}
