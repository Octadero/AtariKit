import XCTest
import AtariKit

class AtariKitTests: XCTestCase {
    func testGame() {
        do {
            print("Set right path to the rom!")
            let game = try Environment(romPath: "/AtariKit/atari-roms/pong.bin")
            
            var gameNumber = 1
            var frame: UInt64 = 0
            while true {
                let reward = game.send(action: .playerAleft)
                let screen = game.screenRGBBuffer()
                let size = game.screenSize()
                
                if screen.count != size.width * size.height * 3 {
                    XCTFail("Incorrect RGB buffer size.")
                }
                //save image on file system and put to ffmpeg
                //game.saveScreen(at: "/tmp/frame_\(frame).png")
                //ffmpeg -r 60 -i /tmp/frame_%d.png  -f mov -c:a mp3 -c:v libx264 -pix_fmt yuv420p agent.mp4
                //
                frame += 1
                if reward != 0 {
                    game.saveScreen(at: "/tmp/image-\(Date().timeIntervalSince1970)-width-\(size.width)xheight-\(size.height).png")
                    print("reward: \(reward).")
                }
                if game.isOver {
                    game.saveScreen(at: "/tmp/image-\(Date().timeIntervalSince1970)-width-\(size.width)xheight-\(size.height).png")

                    gameNumber -= 1
                    game.reset()
                    print("reset game.")
                }
                
                if gameNumber == 0 {
                    print("Done.")
                    break
                }
            }
            
        } catch {
            XCTFail(error.localizedDescription)
        }
    }
    
    
    static var allTests = [
        ("testGame", testGame),
    ]
}
