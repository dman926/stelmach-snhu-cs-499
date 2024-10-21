/**
 * Developer: Dennis James Stelmach
 * Date: 2024-10-04
 * Version: 2.0
 *
 * Revision of the thermostat program provides a clean structure
 * for future development, configuration, and readability.
 *
 * The revision also provides for better safety by simplifying the controls
 * to prevent the thermostat from dangerously calling for heat
 */

#include <stdint.h>
#include <stddef.h>
#include <limits.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define DISPLAY(x) UART_write(uart, &output, x);

/* Configuration */

// Setpoint and current temperature of the thermostat
// in degrees C to control the heating element
volatile unsigned char setpoint = 24;
volatile unsigned char temperature = 0;
// Software lockout temperature in degrees C for safety
// prevents calling for heat when temperature is too high
const int16_t MAX_TEMP = 99;

// THERM_state is the current state of the heating control
enum THERM_states
{
    THERM_IDLE = 0,
    THERM_LOCKOUT = 1,
    THERM_HEAT_OFF = 2,
    THERM_HEAT_ON = 3
} volatile THERM_state = THERM_IDLE;

// Constants for button hold acceleration
const float INITIAL_SPEED = 0.33f;           // Start at 1 update per 3 seconds
const float FINAL_SPEED = 1.0f;              // End at 1 update per second
const float ACCELERATION_DURATION = 5000.0f; // Time to reach max speed (ms)
const unsigned long TIMER_PERIOD = 100;      // Timer period (ms)

// Button state tracking
volatile struct
{
    unsigned char isPressed;
    unsigned long pressStartTime;
    float currentSpeed; // Updates per second
    unsigned long lastUpdateTime;
} Button0 = {0, 0, 0.0f, 0}, Button1 = {0, 0, 0.0f, 0};

// UART Global Variables
char output[64];
int bytesToSend;

// I2C Global Variables
static const struct
{
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = {
    {0x48, 0x0000, "11x"},
    {0x49, 0x0000, "116"},
    {0x41,
     0x0001,
     "006"},
};
uint8_t txBuffer[1];
uint8_t rxBuffer[2];
I2C_Transaction i2cTransaction;

// Driver Handles - Global variables
UART_Handle uart;
I2C_Handle i2c;
Timer_Handle timer500ms;
Timer_Handle timer1000ms;

void initTimer(Timer_Handle *timer, period_ms period, TimerCallbackFxn timerCallback)
{
    Timer_Params params;
    // Init the driver
    Timer_init();
    // Configure the driver
    Timer_Params_init(&params);
    params.period = period * 1000;
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;
    // Open the driver
    *timer = Timer_open(CONFIG_TIMER_0, &params);
    if (timer == NULL)
    {
        int err = snprintf(output, 64, "%dms Timer init error\n\r", period);

        /* Failed to initialize timer */
        while (1)
        {
            // output to uart
            DISPLAY(err)
        }
    }
    if (Timer_start(*timer) == Timer_STATUS_ERROR)
    {
        int err = snprintf(output, 64, "%dms Timer start error\n\r", period);

        /* Failed to start timer */
        while (1)
        {
            // output to uart
            DISPLAY(err)
        }
    }
}

void initUART(void)
{
    UART_Params uartParams;
    // Init the driver
    UART_init();
    // Configure the driver
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.baudRate = 115200;
    // Open the driver
    uart = UART_open(CONFIG_UART_0, &uartParams);
    if (uart == NULL)
    {
        /* UART_open() failed */
        while (1)
            ; // Unable to output error to UART as it did not open
    }
}

void initI2C(void)
{
    uint8_t i, found;
    I2C_Params i2cParams;

    DISPLAY(snprintf(output, 64, "Iniitalizing I2C Driver - "))

    // init driver
    I2C_init();

    // Configure Driver
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;

    // Open driver
    i2c = I2C_open(CONFIG_I2C_0, &i2cParams);
    if (i2c == NULL)
    {
        DISPLAY(snprintf(output, 64, "Failed\n\r"))
        while (1)
            ;
    }

    DISPLAY(snprintf(output, 32, "Passed\n\r"))

    i2cTransaction.writeBuf = txBuffer;
    i2cTransaction.writeCount = 1;
    i2cTransaction.readBuf = rxBuffer;
    i2cTransaction.readCount = 0;

    found = false;
    for (i = 0; i < 3; i++)
    {
        i2cTransaction.slaveAddress = sensors[i].address;
        txBuffer[0] = sensors[i].resultReg;

        DISPLAY(snprintf(output, 64, "Is this %s ", sensors[i].id))
        if (I2C_transfer(i2c, &i2cTransaction))
        {
            DISPLAY(snprintf(output, 64, "Found\n\r"));
            found = true;
            break;
        }
        DISPLAY(snprintf(output, 64, "No\n\r"))
    }

    if (found)
    {
        DISPLAY(snprintf(output, 64, "Detected TMP%s I2C address: %x\n\r",
                         sensors[i].id, i2cTransaction.slaveAddress))
    }
    else
    {
        DISPLAY(snprintf(output, 64,
                         "Temperature sensor not found\n\r"))
    }
}

int16_t readTemp(void)
{
    int16_t temperature = 0;

    i2cTransaction.readCount = 2;
    if (I2C_transfer(i2c, &i2cTransaction))
    {
        // Extract degrees C from data
        // See TMP sensor datasheet
        temperature = (rxBuffer[0] << 8) | (rxBuffer[1]);
        temperature *= 0.0078125;

        // if MSB is 1, then we need to get 2's complement
        if (rxBuffer[0] & 0x80)
        {
            temperature |= 0xF000;
        }
    }
    else
    {
        DISPLAY(snprintf(output, 64,
                         "Error reading temperature sensor (%d)\n\r",
                         i2cTransaction.status))
        DISPLAY(snprintf(
            output,
            64,
            "Please power cycle your board by unplugging USB and plugging back in.\n\r"))
    }

    return temperature;
}

// Helper function to get current time in milliseconds
unsigned long getCurrentTimeMs()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
}

// Linear interpolation helper
float lerp(float start, float end, float t)
{
    if (t < 0.0f)
        return start;
    if (t > 1.0f)
        return end;
    return start + (end - start) * t;
}

// Handle button press with acceleration for setpoint adjustment
void handleButtonHold(volatile struct Button *button, int increment)
{
    unsigned long currentTime = getCurrentTimeMs();

    if (button->isPressed)
    {
        float holdDuration = (float)(currentTime - button->pressStartTime);
        float t = holdDuration / ACCELERATION_DURATION;
        button->currentSpeed = lerp(INITIAL_SPEED, FINAL_SPEED, t);

        float timeSinceLastUpdate = (float)(currentTime - button->lastUpdateTime) / 1000.0f;
        if (timeSinceLastUpdate >= (1.0f / button->currentSpeed))
        {
            int newSetpoint = setpoint + increment;
            if (newSetpoint >= 0 && newSetpoint <= MAX_TEMP)
            {
                setpoint = newSetpoint;
                button->lastUpdateTime = currentTime;
            }
        }
    }
}

// Process temperature state machine transitions
void TickFct_Therm()
{
    temperature = readTemp();

    if (temperature > MAX_TEMP)
    {
        // FAILSAFE when unsafe temps are detected
        THERM_state = THERM_LOCKOUT;
    }

    switch (THERM_state)
    {
    case THERM_IDLE:
        if (temperature < setpoint)
        {
            THERM_state = THERM_HEAT_ON;
        }
        else
        {
            THERM_state = THERM_HEAT_OFF;
        }
        break;

    case THERM_HEAT_ON:
        if (temperature >= setpoint)
        {
            THERM_state = THERM_HEAT_OFF;
        }
        break;

    case THERM_HEAT_OFF:
        if (temperature < setpoint)
        {
            THERM_state = THERM_HEAT_ON;
        }
        break;

    case THERM_LOCKOUT:
        // Only allow reset to OFF state when temps are safe again
        if (temperature < MAX_TEMP - 2)
        {
            THERM_state = THERM_HEAT_OFF;
        }
        break;
    }
}

// Execute state actions based on current state
void TickFct_Heat()
{
    switch (THERM_state)
    {
    case THERM_HEAT_ON:
        // Enable "heating control"
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        break;

    case THERM_HEAT_OFF:
        // Ensure "heating control" is off
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        break;

    case THERM_LOCKOUT:
        // Ensure "heating control" is off
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        break;

    case THERM_IDLE:
        // Ensure "heating control" is off
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        break;
    }
}

// Button handlers
void gpioButtonFxn0(uint_least8_t index)
{
    Button0.isPressed = 1;
    Button0.pressStartTime = getCurrentTimeMs();
    Button0.currentSpeed = INITIAL_SPEED;
    Button0.lastUpdateTime = Button0.pressStartTime;

    // Immediate first increment
    if (setpoint < MAX_TEMP)
    {
        setpoint++;
        Timer500ms(NULL);
    }
}

void gpioButtonFxn1(uint_least8_t index)
{
    Button1.isPressed = 1;
    Button1.pressStartTime = getCurrentTimeMs();
    Button1.currentSpeed = INITIAL_SPEED;
    Button1.lastUpdateTime = Button1.pressStartTime;

    // Immediate first decrement
    if (setpoint > 0)
    {
        setpoint--;
        Timer500ms(NULL);
    }
}

void gpioButtonReleaseFxn0(uint_least8_t index)
{
    Button0.isPressed = 0;
}

void gpioButtonReleaseFxn1(uint_least8_t index)
{
    Button1.isPressed = 0;
}

// Timer callback handlers

// 500ms timer to execute state actions
void Timer500ms(Timer_Handle myHandle)
{
    // Handle button holds
    handleButtonHold(&Button0, 1);  // Increment
    handleButtonHold(&Button1, -1); // Decrement

    // Update thermostat state
    TickFct_Therm();
    // Execute actions for current state
    TickFct_Heat();
}

unsigned long startTimeSeconds = 0;

// 1000ms timer to output current state to UART for monitoring
void Timer1000ms(Timer_Handle myHandle)
{
    if (startTimeMS == 0)
    {
        startTimeMS = getCurrentTimeMs();
    }
    unsigned long seconds = (getCurrentTimeMs() - startTimeMS) / 1000;
    // <TEMP,SETPOINT,HEAT,SECONDS>
    // TEMP - current temperature in degrees C
    // SETPOINT - setpoint temperature in degrees C
    // HEAT - heating state (off/on)
    // SECONDS - uptime of device
    DISPLAY(snprintf(output, 64, "<%02d,%02d,%d,%04d>\n\r",
                     temperature, setpoint, !(THERM_state - 1), seconds));
}

void *mainThread(void *arg0)
{
    // Initialize GPIO
    GPIO_init();

    // Configure LED to act in place of a heating control output pin
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);

    // Configure buttons with interrupts for both press and release
    // (rising and falling edges)
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0,
                   GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_BOTH_EDGES);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_1,
                   GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_BOTH_EDGES);

    // Install button callbacks
    // On press
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);
    GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
    // On release
    GPIO_setCallbackRising(CONFIG_GPIO_BUTTON_0, gpioButtonReleaseFxn0);
    GPIO_setCallbackRising(CONFIG_GPIO_BUTTON_1, gpioButtonReleaseFxn1);

    // Enable button interrupts
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);
    GPIO_enableInt(CONFIG_GPIO_BUTTON_1);

    // Initialize other peripherals
    initUART(); // UART for debugging
    initI2C();  // I2C for temperature sensor

    // Configure two hardware timers

    Timer_Params params;
    Timer_Params_init(&params);

    params.period = 500000 * 1000; // 500ms in microseconds
    Timer_Handle timer500ms = Timer_create(Timer500ms, &params, NULL);

    params.period = 1000000; // 1000ms
    Timer_Handle timer1000ms = Timer_create(Timer1000ms, &params, NULL);

    initTimer(&timer500ms, 500, Timer500ms);
    initTimer(&timer1000ms, 1000, Timer1000ms);

    // Start timers
    Timer_start(timer500ms);
    Timer_start(timer1000ms);

    // Main loop - can sleep or handle other tasks
    while (1)
    {
        sleep(1);
    }

    return (NULL);
}