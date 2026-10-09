# STM32F446RE Bare-Metal USART2 Driver with EXTI Hardware Interrupts

A register-level Bare-Metal C firmware implementation demonstrating UART asynchronous serial communication integrated with PC13 push-button external line hardware interrupts (EXTI15_10) on the STM32F446RE microcontroller.

---

## 📌 Technical Highlights

* **Pure Bare-Metal C:** Direct register-level address mapping without vendor abstraction layers (STM32 HAL / LL).
* **USART Peripheral Config:** Configured USART2 TX for standard asynchronous communication at **115200 baud** (16 MHz APB1 peripheral clock).
* **GPIO Alternate Function:** Configured `PA2` to Alternate Function 7 (`AF7`) for USART2 TX output.
* **EXTI Interrupt Line Mapping:** Configured `PC13` user push-button to trigger `EXTI15_10` vector via `SYSCFG_EXTICR4` multiplexer on falling edge transition.
* **NVIC Control:** Programmed Cortex-M4 Nested Vectored Interrupt Controller (`NVIC_ISER1`) to handle EXTI line 13 IRQ asynchronously.

---

## 🛠 Circuit & Pin Mapping

* **Microcontroller:** STM32F446RE (ARM Cortex-M4)
* **Pin Allocation:**
  * `PA2`: USART2_TX (Alternate Function `AF7`)
  * `PC13`: Blue User Push-Button (Input with Falling Edge EXTI Trigger)
* **Baudrate:** 115200 bps

---

## 📁 Repository Structure

* `main.c` - Complete firmware containing USART2 register setups, baudrate divider calculation routines, EXTI configuration, and IRQ Handler (`EXTI15_10_IRQHandler`).
