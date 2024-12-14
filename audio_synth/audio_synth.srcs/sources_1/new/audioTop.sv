module audioTop(
    input clk_100MHz,
    output SPKL,
    output SPKR,
    input [15:0] sw_i,
    input reset_rtl_0,
    input UART_TXD,
    output UART_RXD,
    input leButton,
    output led0,
    output led15,
    output [3:0] hex_gridA,
    output [7:0] hex_segA,
    output [3:0] hex_gridB,
    output [7:0] hex_segB
    );
  
  logic [2:0] volume;
  logic [15:0] frequency;
  mb_block mb_block_i
   (.clk_100MHz(clk_100MHz),
    .gpio_rtl_1_tri_i(sw_i),
    .gpio_rtl_2_tri_i(leButton),
    .reset_rtl_0(~reset_rtl_0),
    .uart_rtl_0_rxd(UART_TXD),
    .uart_rtl_0_txd(UART_RXD),
    .gpio_rtl_0_tri_o(frequency),
    .gpio_rtl_3_tri_o(volume));
   
   logic [3:0] inA[4];
   assign inA[0] = frequency[15:12];
   assign inA[1] = frequency[11:8];
   assign inA[2] = frequency[7:4];
   assign inA[3] =frequency[3:0] ;
   
   hex_driver hex_driverA (
    .clk(clk_100MHz),
    .reset(reset_rtl_0),

    .in(inA),

    .hex_seg(hex_segA),
    .hex_grid(hex_gridA)
    );
    
    
   logic [3:0] inB[4];
   assign inB[0] = {3'b0, SPKL};
   assign inB[1] = {3'b0, SPKR};
   assign inB[2] = {3'b0, leButton};
   assign inB[3] ={1'b0, volume[2:0]} ;
     
   hex_driver hex_driverB (
    .clk(clk_100MHz),
    .reset(reset_rtl_0),

    .in(inB),

    .hex_seg(hex_segB),
    .hex_grid(hex_gridB)
    );
    
   pwm_sine_wave pwm_sine_wave (
    .clk_100MHz(clk_100MHz),  // Input 
    .freq(frequency),
    .volume(volume),
    .SPKL(SPKL),       // PWM output for left channel
    .SPKR(SPKR)        // PWM output for right channel
    );
     
    
  assign led0 = 1;
endmodule
