module hardware_only_audio(
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
  
  // Array of frequencies
  localparam int TAB_SIZE = 35; 
  localparam logic [15:0] frequencies[TAB_SIZE] = '{
      568, 536, 1080, 808, 672, 1072, 800,
      679, 672, 568, 1136, 856, 720, 712,
      848, 711, 720, 216, 536, 1072, 808,
      672, 1080, 799, 680, 672, 640, 720,
      720, 808, 800, 855, 896, 1072, 264
  };

  logic [2:0] volume;
  logic [15:0] frequency = 0;
  logic [31:0] counter;
  logic [5:0] i;
  logic lock = 0;

  assign volume = sw_i[2:0];

  // ----------------------------------------
  // Button Press Detection
  // ----------------------------------------
  logic leButton_prev = 0; 
  logic button_pressed = 0; 
  always_ff @(posedge clk_100MHz) begin
    if (leButton == 1 && leButton_prev == 0) 
      button_pressed <= 1;
    else
      button_pressed <= 0;
      
    leButton_prev <= leButton;
  end

  always_ff @(posedge clk_100MHz or posedge reset_rtl_0) begin
    if (reset_rtl_0) begin
      counter <= 0;
      i <= 0;
      frequency <= frequencies[0];
    end else if (!lock) begin
      // If lock is 0, reset the sequence
      counter <= 0;
      i <= 0;
      frequency <= frequencies[0];
    end else if (button_pressed) begin
      // If the button is pressed again while playing, restart sequence
      counter <= 0;
      i <= 0;
      frequency <= frequencies[0];
    end else begin
      // Normal operation: increment counter and cycle through frequencies
      if (counter == 5_000_000) begin
        counter <= 0;
        i <= (i + 1) % TAB_SIZE;
        frequency <= frequencies[i];
      end else begin
        counter <= counter + 1;
      end
    end
  end

  logic [31:0] lock_counter = 0;
  always_ff @(posedge clk_100MHz) begin
    if (button_pressed) begin
      lock <= 1;
      lock_counter <= 0;
    end else if (lock == 1) begin
      if (lock_counter == 40_000_000) begin
        lock <= 0;
        lock_counter <= 0;
      end else begin
        lock_counter <= lock_counter + 1;
      end
    end
  end

  // ----------------------------------------
  // Hex driver A logic
  // ----------------------------------------
  logic [3:0] inA[4];
  assign inA[0] = frequency[15:12];
  assign inA[1] = frequency[11:8];
  assign inA[2] = frequency[7:4];
  assign inA[3] = frequency[3:0];
   
  hex_driver hex_driverA (
    .clk(clk_100MHz),
    .reset(reset_rtl_0),
    .in(inA),
    .hex_seg(hex_segA),
    .hex_grid(hex_gridA)
  );

  // ----------------------------------------
  // Hex driver B logic
  // ----------------------------------------
  logic [3:0] inB[4];
  assign inB[0] = {3'b0, SPKL};
  assign inB[1] = {3'b0, SPKR};
  assign inB[2] = {3'b0, leButton};
  assign inB[3] = {1'b0, volume[2:0]};
     
  hex_driver hex_driverB (
    .clk(clk_100MHz),
    .reset(reset_rtl_0),
    .in(inB),
    .hex_seg(hex_segB),
    .hex_grid(hex_gridB)
  );

  // ----------------------------------------
  // PWM Sine Wave Generation
  // ----------------------------------------
  pwm_sine_wave pwm_sine_wave (
    .clk_100MHz(clk_100MHz),
    .freq(frequency),
    .volume(volume),
    .SPKL(SPKL),
    .SPKR(SPKR),
    .lock(lock)
  );

  assign led0 = 1;

endmodule
