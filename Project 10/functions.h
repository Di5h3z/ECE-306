//------------------------------------------------------------------------------
//  Description: This file contains the Function prototypes
//
//By:           Nathan Carels
//Date:         3/9/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// My Functions
//------------------------------------------------------------------------------

//IOT
void reverse(int time);
void left_turn(int time);
void right_turn(int time);
void forward(int time);
int str_to_int(char*value);
char* verify_pin(char*command);
char capture_IP(char current);
char* get_upper_IP(void);
char* get_lower_IP(void);

//Serial
  void Init_Serial(void);
  void Init_Serial_UCA1(void);
  void Init_Serial_UCA0(void);
  void USB_tx(char* command);
  void IOT_tx(char* command);
  char* USB_rx(void);
  char* IOT_rx(void);
  
  extern volatile char USB_message_ready;
  extern volatile char USB_transmitting;
  extern volatile char IOT_message_ready;
  extern volatile char IOT_transmitting;
  
//Init
  void Init_REF(void);
  void Init_Conditions(void);
  void Init_LEDs(void);

//LCD
  void clear_lcd(void);
  void lcd_line1(char* line);
  void lcd_line2(char* line);
  void lcd_line3(char* line);
  void lcd_line4(char* line);
//Drive
  void L_stop(void);
  void R_stop(void);
  void L_forward(unsigned int speed);
  void R_forward(unsigned int speed);
  void L_reverse(unsigned int speed);
  void R_reverse(unsigned int speed);
//Timers
  void Timers_Process(void);
  void Init_Timers(void);
  void Init_Timer_B0(void);
  void Init_Timer_B1(void);

  extern unsigned int Second_Count;
//ADC
  void Init_ADC(void);
  void display_averages(void);
  int return_vleft_average(void);
  int return_vright_average(void);
  int return_vthumb_average(void);
  void HEXtoBCD(int hex_value);
//Error
  void wheel_polarity_error(void);
//Menu
  void str_cpy(char* str_copy, char* str_orig);
  void menu(void);

  void assign_static_strings(void);
  void put_screen(void);
  void display_state(void);
  void blink_selected(void);
  void handle_select(void);
  
  void display_clock(void);
  void set_clock(int setting);
  int get_clock_time(void);
  void disable_clock(void);
  void enable_clock(void);

  extern char update_menu;
  extern char* screen1_line1;
  extern char* screen1_line2;
  extern char* screen1_line3;
  extern char* screen1_line4;

  extern char* screen2_line1;
  extern char* screen2_line2;
  extern char* screen2_line3;
  extern char* screen2_line4;

  extern char* screen3_line1;
  extern char* screen3_line2;
  extern char* screen3_line3;
  extern char* screen3_line4;

  extern char* screen4_line1;
  extern char* screen4_line2;
  extern char* screen4_line3;
  extern char* screen4_line4;

//DAC
  void Init_DAC(void);
//PID NAV
  int pid_process(void);
  void pid_display(void);
  void line_nav(int speed);
  char get_state(void);

//------------------------------------------------------------------------------
// Other Functions
//------------------------------------------------------------------------------
// Interrupts
void enable_interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void switch_interrupt(void);

// Analog to Digital Converter

// Clocks
void Init_Clocks(void);

// LED Configurations
void IR_LED_control(char selection);
void Backlite_control(char selection);

  // LCD
void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);

void lcd_power_on(void);
void lcd_write_line1(void);
void lcd_write_line2(void);
//void lcd_draw_time_page(void);
//void lcd_power_off(void);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);
//void lcd_write(unsigned char c);
//void out_lcd(unsigned char c);

void Write_LCD_Ins(char instruction);
void Write_LCD_Data(char data);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);

void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);
void lcd_rotate(char view);

//void lcd_write(char data, char command);
void lcd_write(unsigned char c);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_write_line3(void);

void lcd_command( char data);
void LCD_test(void);
void LCD_iot_meassage_print(int nema_index);

// Menu
void Menu_Process(void);

// Ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// SPI
void Init_SPI_B1(void);
void SPI_B1_write(char byte);
void spi_rs_data(void);
void spi_rs_command(void);
void spi_LCD_idle(void);
void spi_LCD_active(void);
void SPI_test(void);
void WriteIns(char instruction);
void WriteData(char data);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);
void Init_Switch(void);
void Switch_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);
void menu_act(void);
void menu_select(void);

// Timers
void Init_Timer_B2(void);
void Init_Timer_B3(void);

void usleep(unsigned int usec);
void usleep10(unsigned int usec);
void five_msec_sleep(unsigned int msec);
void measure_delay(void);
void out_control_words(void);
