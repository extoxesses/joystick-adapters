// --- PS2 input data dependencies ----
#include <PS2KeyAdvanced.h>
#include <PS2KeyMap.h>

// --- USB output data dependencies ---
#include <Keyboard.h>
#include <Mouse.h>

// --- PS2 Keyboard properties --------
#define DEBUG 0                    // 0 = False | 1 = True
#define PS2_MOUSE_PIN 2            // IRQ enabled pin
#define PS2_CLOCK_PIN 3            // IRQ enabled pin
#define PS2_DATA_PIN 4             
// --- CAPS usage properties ----------
#define MAX_MODIFIERS_COMBO 5      // Normal combos don't require more than 3 caps. Choose 5 to be more resilient
#define DEFAULT_DELAY 5            // Consider 5 millis as the time need to push a cap
#define MAX_COMBO_DELTA_MILLIS 300 // Empiric value that allows to perform a combo without introducint to much latency on single cap push

// --- Skretch properties
PS2KeyAdvanced ps_2_in;
PS2KeyMap keymap;

uint16_t modifiers[MAX_MODIFIERS_COMBO];
short modifiers_idx = -1;
unsigned long last_mod_millis = -1;
uint8_t lock_status = 0;

bool mouse_pressed = false;

// --- Setup -------------

void ps2KeyboardSetup() {
  ps_2_in.begin(PS2_DATA_PIN, PS2_CLOCK_PIN);
  ps_2_in.setNoBreak(1);
  ps_2_in.setNoRepeat(1);
  ps_2_in.typematic(0, 1);
  ps_2_in.resetKey();
  ps_2_in.read();

  verifyPs2KeyboardConnection();
  keymap.selectMap((char *) "US");
}

void mouseSetup() {
  attachInterrupt(digitalPinToInterrupt(PS2_MOUSE_PIN), mousePress, FALLING);
  pinMode(10, OUTPUT);
  digitalWrite(10, LOW);
}

// --- Remapping ---------

void mapAndSend(uint16_t cap) {
  uint16_t translate;

  if (4161 <= cap && cap <= 4186) {
    cap -= 4096;  // Converts capitol letter into lower case letter (for capslock management)
  } else if (12353 <= cap && cap <= 12378) {
    cap -= 12288; // Converts capitol letter into lower case letter (for capslock management)
  }

  switch (cap) {
    case 6:
      translate = KEY_PAUSE;
      break;
    case 43:  // With caps lock active
    case 286:
    case 4139:  // With caps lock active
    case 4382:  // With caps lock active
      translate = KEY_KP_ENTER;
      break;

    case 44:                                                                                                                                                                                                                                                                         
    case 4140:  // With caps lock active
      translate = KEY_KP_PLUS;
      break;
    case 45:
    case 4141:  // With caps lock active
      translate = KEY_KP_MINUS;
      break;
    case 46:
    case 4142:  // With caps lock active
      translate = KEY_KP_ASTERISK;
      break;
    case 47:
    case 4143:  // With caps lock active
      translate = KEY_KP_SLASH;
      break;

    case 260:
    case 4356:  // With caps lock active
      translate = KEY_PRINT_SCREEN;
      break;

    case 273:
    case 4369:  // With caps lock active
      translate = KEY_HOME;
      break;
    case 274:
    case 4370:  // With caps lock active
      translate = KEY_END;
      break;
    case 275:
    case 4371:  // With caps lock active
      translate = KEY_PAGE_UP;
      break;
    case 276:
    case 4372:  // With caps lock active
      translate = KEY_PAGE_DOWN;
      break;
    case 277:
    case 4373:  // With caps lock active
    case 16661: // Shift
    case 20757: // Caps Lock + Shift
      translate = KEY_LEFT_ARROW;
      break;
    case 278:
    case 4374:  // With caps lock active
    case 16662: // Shift
    case 20758: // Caps Lock + Shift
      translate = KEY_RIGHT_ARROW;
      break;
    case 279:
    case 4375:  // With caps lock active
    case 16663: // Shift
    case 20759: // Caps Lock + Shift
      translate = KEY_UP_ARROW;
      break;
    case 280:
    case 4376:  // With caps lock active
    case 16664: // Shift
    case 20760: // Caps Lock + Shift
      translate = KEY_DOWN_ARROW;
      break;
    case 281:
    case 4377:  // With caps lock active
      translate = KEY_INSERT;
      break;
    case 282:
    case 4378:  // With caps lock active
      translate = KEY_DELETE;
      break;

    case 283:
    case 4379:  // With caps lock active
      translate = KEY_ESC;
      break;
    case 353:
    case 4449:  // With caps lock active
      translate = KEY_F1;
      break;
    case 354:
    case 4450:  // With caps lock active
      translate = KEY_F2;
      break;
    case 355:
    case 4451:  // With caps lock active
      translate = KEY_F3;
      break;
    case 356:
    case 4452:  // With caps lock active
      translate = KEY_F4;
      break;
    case 357:
    case 4453: // With caps lock active
    case 393:  // Refresh command
    case 4489: // CapsLock + Refresh command
      translate = KEY_F5;
      break;
    case 358:
    case 4454:  // With caps lock active
      translate = KEY_F6;
      break;
    case 359:
    case 4455:  // With caps lock active
      translate = KEY_F7;
      break;
    case 360:
    case 4456:  // With caps lock active
      translate = KEY_F8;
      break;
    case 361:
    case 4457:  // With caps lock active
      translate = KEY_F9;
      break;
    case 362:
    case 4458:  // With caps lock active
      translate = KEY_F10;
      break;
    case 363:
    case 4459:  // With caps lock active
      translate = KEY_F11;
      break;
    case 364:
    case 4460:  // With caps lock active
      translate = KEY_F12;
      break;

    case 390:
    case 4486:  // With caps lock active
      altCombo(KEY_LEFT_ARROW);
      return;
    case 391:
    case 4487:  // With caps lock active
      altCombo(KEY_RIGHT_ARROW);
      return;
    case 2404:
    case 6500:  // With caps lock active
      altCombo(KEY_F4);
      return;
    case 18694:
    case 22790:  // With caps lock active
      altCombo(KEY_TAB);
      return;

    case 780:
    case 4876:  // With caps lock active
      handleModifier(KEY_LEFT_GUI);
      return;
    case 781:
    case 4877:  // With caps lock active
      handleModifier(KEY_RIGHT_GUI);
      return;

    case 16646: // Shift Left
    case 20742: // CapsLock + Shift Left
    case 24838: // Ctrl + Shift Left
    case 28934: // CapsLock + Ctrl + Shift Left
    case 26886: // Ctrl + Shift Left + Alt Left
    case 30982: // CapsLock + Ctrl + Shift Left + Alt Left
      handleModifier(KEY_LEFT_SHIFT);
      return;
    case 8456:  // Ctrl Left
    case 12552: // CapsLock + Ctrl Left
      handleModifier(KEY_LEFT_CTRL);
      return;
    case 2314:  // Alt Left
    case 6410:  // CapsLock + Alt Left
    case 10506: // Ctrl Left + Alt Left
    case 14602: // CapsLock + Ctrl Left + Alt Left
      handleModifier(KEY_LEFT_ALT);
      return;
    case 16647: // Shift Right
    case 20743: // CapsLock + Shift Right
    case 24839: // Ctrl Right + Shift Right
    case 28935: // CapsLock + Ctrl Right + Shift Right
    case 17671: // AltGr + Shift Right
    case 21767: // CapsLock + AltGr + Shift Right
      handleModifier(KEY_RIGHT_SHIFT);
      return;
    case 8457:  // Ctrl Right
    case 12553: // CapsLock + Ctrl Right
      handleModifier(KEY_RIGHT_CTRL);
      return;
    case 1291: // AltGr
    case 5387: // With caps lock active
      handleModifier(KEY_RIGHT_ALT);
      return;
    case 270:
    case 4366:
      translate = KEY_MENU;
      break;    

    case 16666:
    case 20762:
      translate = 'x';
      break;

    case 250:
      handleLocks();
      return;
    default:
      translate = keymap.remapKey(cap);
      break;
  }

  printChar(cap, translate);
  keyPress(translate, DEFAULT_DELAY);
  releaseAllModifiers();
}

// --- Press emulation ---

void altCombo(uint8_t cap) {
  serialPrint(cap);

  Keyboard.press(KEY_LEFT_ALT);
  Keyboard.press(cap);
  delay(DEFAULT_DELAY);
  Keyboard.release(cap);
  Keyboard.release(KEY_LEFT_ALT);
}

void handleLocks() {
  uint8_t locks = ps_2_in.getLock();
  uint8_t delta = abs(locks - lock_status);

  if (delta) {
    lock_status = locks;

    switch(delta) {
      case 1:
        keyPress(KEY_SCROLL_LOCK, DEFAULT_DELAY);
        break;
      case 2:
        keyPress(KEY_NUM_LOCK, DEFAULT_DELAY);
        break;
      case 4:
        keyPress(KEY_CAPS_LOCK, DEFAULT_DELAY);
        break;
      default:
        // do nothing
        break;
    }  
  }
}

void handleModifier(uint16_t modifier) {
  serialPrint(modifier);

  if ( modifiers_idx == MAX_MODIFIERS_COMBO - 1 ) {
    return;
  }

  modifiers[++modifiers_idx] = modifier;
  Keyboard.press(modifier);
  last_mod_millis = millis();
}

void handleModifierRelease() {
  if (modifiers_idx > -1
      && (millis() - last_mod_millis) >= MAX_COMBO_DELTA_MILLIS) {

    releaseAllModifiers();
  }
}

void keyPress(uint16_t cap, unsigned long ms) {
  Keyboard.press(cap);
  delay(ms);
  Keyboard.release(cap);
}

void mousePress() {
  #if DEBUG
    Serial.print("Mouse click: ");
    Serial.println(mouse_pressed);
  #endif

  if (mouse_pressed) {
    Mouse.release(MOUSE_MIDDLE);
  } else {
    Mouse.press(MOUSE_MIDDLE);
  }
  mouse_pressed = !mouse_pressed;
  delay(250);
}

void releaseAllModifiers() {
  while(modifiers_idx >= 0) {
    #if DEBUG
      Serial.print("Releasing ");
      Serial.println(modifiers[modifiers_idx]);
    #endif

    Keyboard.release(modifiers[modifiers_idx--]);
  }
}

// --- Debugger ----------

void printChar(uint16_t value, uint16_t map) {
  #if DEBUG
    Serial.print(value);
    Serial.print(" | ");
    Serial.print(value, HEX);
    Serial.print(" | ");
    Serial.print((char)value);
    Serial.print(" | -> | ");
    Serial.print(map);
    Serial.print(" | ");
    Serial.print(map, HEX);
    Serial.print(" | ");
    Serial.println((char)map);
  #endif
}

void serialPrint(uint16_t value) {
  #if DEBUG
    Serial.print("Modifier: ");
    Serial.println(value);
  #endif
}

void verifyPs2KeyboardConnection() {
  #if DEBUG
    ps_2_in.echo();
    uint16_t code = ps_2_in.read();

    // Check keyboard
    if ((code & 0xFF) == PS2_KEY_ECHO || (code & 0xFF) == PS2_KEY_BAT) {
      // Response was Echo or power up
      Serial.println("Keyboard OK");
    } else if ((code & 0xFF) == 0) {
      Serial.println("Keyboard Not Found");
    } else {
      Serial.print("Invalid Code received of ");
      Serial.println(code, HEX);
    }

    // Verify keyboard scan code
    ps_2_in.getScanCodeSet();
    code = ps_2_in.read();
    Serial.print("ScanCodeSet: ");
    Serial.println(code, HEX);
  #endif
}



void setup() {
  #if DEBUG
    Serial.begin(115200);
  #endif

  // Configure the input keyboard
  ps2KeyboardSetup();

  // Configure the output interface
  //mouseSetup();

  Keyboard.begin();
  //Mouse.begin();
  delay(10);
}

void loop() {
  if (ps_2_in.available()) {
    mapAndSend(ps_2_in.read());
  }
  handleModifierRelease();
}
