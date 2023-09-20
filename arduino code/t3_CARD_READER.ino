#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9           // Configurable, see typical pin layout above
#define SS_PIN          10          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

// Number of known default keys (hard-coded)
// NOTE: Synchronize the NR_KNOWN_KEYS define with the defaultKeys[] array
#define NR_KNOWN_KEYS   8
// Known keys, see: https://code.google.com/p/mfcuk/wiki/MifareClassicDefaultKeys
byte knownKeys[NR_KNOWN_KEYS][MFRC522::MF_KEY_SIZE] =  {
   // {0xff, 0xff, 0xff, 0xff, 0xff, 0xff}, // FF FF FF FF FF FF NEVER USE THESE!
    {0x37, 0x21, 0x53, 0x6a, 0x72, 0x40}, // t3
    {0x57, 0x43, 0x43, 0x46, 0x76, 0x32}, // SAYYYYGAAAAA(does not work.)
    {0x72, 0x61, 0x74, 0x73, 0x61, 0x6d}, // cyclon?
    {0x72, 0x65, 0x74, 0x73, 0x61, 0x6d}, // cyclon????
    {0x21, 0x44, 0x4D, 0x54, 0x31, 0x21}, // T1 need key 21444D543121
    {0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff}, // T1(jp?) need key
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00}  // T2 need key
};

/*
 * Initialize.
 */
void setup() {
    Serial.begin(9600);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card
    Serial.println(F("HELLO!"));
}

/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(char(buffer[i]));
    }
}

/*
 * Try using the PICC (the tag/card) with the given key to access block 1 and block 2.
 * On success, it will show the key details and dump the block data on Serial in a single line.
 *
 * @return true when the given key worked, false otherwise.
 */
boolean try_key(MFRC522::MIFARE_Key *key)
{
    boolean result = false;
    byte buffer[18];
    byte status;

    // Serial.println(F("Authenticating using key A for block 1..."));
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 1, key, &(mfrc522.uid));
    if (status != MFRC522::STATUS_OK) {
        if (!mfrc522.PICC_IsNewCardPresent()) return false;
        if (!mfrc522.PICC_ReadCardSerial()) return false;
        // Serial.print(F("PCD_Authenticate() failed: "));
        // Serial.println(mfrc522.GetStatusCodeName(status));
        return false;
    }

    // Read block 1
    byte byteCount = sizeof(buffer);
    status = mfrc522.MIFARE_Read(1, buffer, &byteCount);
    if (status != MFRC522::STATUS_OK) {
        // Print a generic error message for block 1
        Serial.println();
        Serial.print(F("Authentication failed"));
    }
    else {
        // Successful read
        result = true;
        Serial.print(F("KEY:"));
        
       // dump_byte_array((*key).keyByte, MFRC522::MF_KEY_SIZE);
        Serial.println();
        Serial.print(F("Name:"));
        dump_byte_array(buffer, 16);
    }

    // Read block 2
    status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, 2, key, &(mfrc522.uid));
    if (status == MFRC522::STATUS_OK) {
        // Read block 2
        byteCount = sizeof(buffer);
        status = mfrc522.MIFARE_Read(2, buffer, &byteCount);
        if (status == MFRC522::STATUS_OK) {
            // Successful read
            Serial.print(F(""));
            dump_byte_array(buffer, 16);
        }
        else {
            // Print a generic error message for block 2
            Serial.println();
            Serial.print(F("Authentication failed"));
        }
    }
    else {
        // Print a generic error message for block 2
        Serial.println();
        Serial.print(F("BLOCK READ TIMEOUT"));
    }

    Serial.println(); // Newline after printing both blocks

    mfrc522.PICC_HaltA();       // Halt PICC
    mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD
    return result;
}
/*
 * Main loop.
 */
void loop() {
    // Look for new cards
    if (!mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    Serial.print(F(""));
    Serial.println();
    //dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println();
    Serial.print(F(""));
    byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
    Serial.println(mfrc522.PICC_GetTypeName(piccType));

    // Try the known default keys
    MFRC522::MIFARE_Key key;
    for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
        // Try the key
        if (try_key(&key)) {
            // Found and reported on the key and block,
            // no need to try other keys for this PICC
            delay(5000);
            break;
        }
    }
}