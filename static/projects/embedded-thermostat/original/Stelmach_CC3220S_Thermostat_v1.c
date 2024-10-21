/*
 * See LICENSE file
 */

/**
 * Developer: Dennis James Stelmach
 * Date: 2024-04
 * Version: 1.0
 */

#include <stdint.h>
#include <stddef.h>

/* Driver Header files */
#include <ti/drivers/GPIO.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/drivers/Timer.h>

/* Driver configuration */
#include "ti_drivers_config.h"

#define DISPLAY(x) UART_write(uart, &output, x);

// UART Global Variables
char output[64];
int bytesToSend;

// I2C Global Variables
static const struct
{
    uint8_t address;
    uint8_t resultReg;
    char *id;
} sensors[3] = { { 0x48, 0x0000, "11x" }, { 0x49, 0x0000, "116" }, { 0x41,
                                                                     0x0001,
                                                                     "006" }, };
uint8_t txBuffer[1];
uint8_t rxBuffer[2];
I2C_Transaction i2cTransaction;

// Driver Handles - Global variables
UART_Handle uart;
I2C_Handle i2c;
Timer_Handle timer0;

const unsigned long timerPeriod = 100;
volatile unsigned char TimerFlag = 0;
void timerCallback(Timer_Handle myHandle, int_fast16_t status)
{
    TimerFlag = 1;
}
void initTimer(void)
{
    Timer_Params params;
    // Init the driver
    Timer_init();
    // Configure the driver
    Timer_Params_init(&params);
    params.period = timerPeriod * 1000; // 100ms
    params.periodUnits = Timer_PERIOD_US;
    params.timerMode = Timer_CONTINUOUS_CALLBACK;
    params.timerCallback = timerCallback;
    // Open the driver
    timer0 = Timer_open(CONFIG_TIMER_0, &params);
    if (timer0 == NULL)
    {
        /* Failed to initialized timer */
        while (1)
        {
        }
    }
    if (Timer_start(timer0) == Timer_STATUS_ERROR)
    {
        /* Failed to start timer */
        while (1)
        {
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
            ;
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
                         "Temperature sensor not found, contact professor\n\r"))
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

volatile unsigned char UpFlag = 0;
void gpioButtonFxn0(uint_least8_t index)
{
    UpFlag = 1;
}

volatile unsigned char DownFlag = 0;
void gpioButtonFxn1(uint_least8_t index)
{
    DownFlag = 1;
}

volatile unsigned char setpoint = 24; // Default to standard room temperature in C
enum ADJ_states { ADJ_SMStart, ADJ_NONE, ADJ_UP, ADJ_DOWN } ADJ_state;
void TickFct_Adjust() {
    switch (ADJ_state) {
    case ADJ_SMStart:
        ADJ_state = ADJ_NONE;
        break;
    case ADJ_NONE:
        if (UpFlag && !DownFlag) {
            ADJ_state = ADJ_UP;
        } else if (!UpFlag && DownFlag) {
            ADJ_state = ADJ_DOWN;
        }
        break;
    case ADJ_UP:
        if (!UpFlag) {
            if (DownFlag) {
                ADJ_state = ADJ_DOWN;
            } else {
                ADJ_state = ADJ_NONE;
            }
        }
        break;
    case ADJ_DOWN:
        if (!DownFlag) {
            if (UpFlag) {
                ADJ_state = ADJ_UP;
            } else {
                ADJ_state = ADJ_NONE;
            }
        }
        break;
    }

    switch (ADJ_state) {
    case ADJ_UP:
        if (setpoint < 99) {
            setpoint++;
        }
        UpFlag = 0;
        break;
    case ADJ_DOWN:
        if (setpoint > 0) {
            setpoint--;
        }
        DownFlag = 0;
        break;
    default:
        // Do nothing
        break;
    }
}

volatile unsigned char heat = 0;
volatile unsigned char temperature = 0;
enum HT_states { HT_SMStart, HT_ON, HT_OFF } HT_state;
void TickFct_Heat() {
    temperature = readTemp();
    heat = temperature < setpoint;
    if (heat) {
        HT_state = HT_ON;
    } else {
        HT_state = HT_OFF;
    }

    switch (HT_state) {
    case HT_ON:
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);
        break;
    case HT_OFF:
        GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_OFF);
        break;
    default:
        // Do nothing
        break;
    }
}

volatile unsigned char seconds = 0;
void TickFct_Output() {
    seconds++;
    DISPLAY(snprintf(output, 64, "<%02d,%02d,%d,%04d>\n\r", temperature,
                             setpoint, heat, seconds))
}

/*
 *  ======== mainThread ========
 */
void* mainThread(void *arg0)
{
    /* Call driver init functions */
    GPIO_init();

    /* Configure the LED and button pins */
    GPIO_setConfig(CONFIG_GPIO_LED_0, GPIO_CFG_OUT_STD | GPIO_CFG_OUT_LOW);
    GPIO_setConfig(CONFIG_GPIO_BUTTON_0,
                   GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

    /* Turn on user LED */
    GPIO_write(CONFIG_GPIO_LED_0, CONFIG_GPIO_LED_ON);

    /* Install Button callback */
    GPIO_setCallback(CONFIG_GPIO_BUTTON_0, gpioButtonFxn0);

    /* Enable interrupts */
    GPIO_enableInt(CONFIG_GPIO_BUTTON_0);

    /*
     *  If more than one input pin is available for your device, interrupts
     *  will be enabled on CONFIG_GPIO_BUTTON1.
     */
    if (CONFIG_GPIO_BUTTON_0 != CONFIG_GPIO_BUTTON_1)
    {
        /* Configure BUTTON1 pin */
        GPIO_setConfig(CONFIG_GPIO_BUTTON_1, GPIO_CFG_IN_PU | GPIO_CFG_IN_INT_FALLING);

        /* Install Button callback */
        GPIO_setCallback(CONFIG_GPIO_BUTTON_1, gpioButtonFxn1);
        GPIO_enableInt(CONFIG_GPIO_BUTTON_1);
    }

    initUART();
    initI2C();
    initTimer();

    unsigned long ADJ_elapsedTime = 200;
    unsigned long HT_elapsedTime = 500;
    unsigned long OUT_elapsedTime = 1000;

    while (1)
    {
        while (!TimerFlag) {}
        TimerFlag = 0;
        if (ADJ_elapsedTime >= 200) {
            TickFct_Adjust();
            ADJ_elapsedTime = 0;
        }
        if (HT_elapsedTime >= 500) {
            TickFct_Heat();
            HT_elapsedTime = 0;
        }
        if (OUT_elapsedTime >= 1000) {
            TickFct_Output();
            OUT_elapsedTime = 0;
        }

        ADJ_elapsedTime += timerPeriod;
        HT_elapsedTime += timerPeriod;
        OUT_elapsedTime += timerPeriod;
    }

    return (NULL);
}
