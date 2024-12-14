module pwm_sine_wave (
    input logic clk_100MHz,  // Input 
    input logic [15:0] freq,
    input logic [2:0] volume,
    output logic SPKL,       // PWM output for left channel
    output logic SPKR,        // PWM output for right channel
    input logic lock
);
    logic [7:0] sine_table[0:31]; // Sine wave lookup table (32 steps, 8-bit values)
    logic [4:0] index  = 0;            // Index for sine table
    logic [7:0] duty_cycle = 0;       // Current PWM duty cycle
    logic [18:0] counter = 0;         // Counter for PWM timing  // 18 bits are sufficient to count to 142,857
    // Initialize sine wave lookup table
    //sin(degree) = ((degree+1)/2) * 255
    initial begin
        sine_table[0]  = 128; // Sin(0°) = 0.5 (normalized to 8-bit range)
        sine_table[1]  = 153; // Sin(11.25°)
        sine_table[2]  = 178; // Sin(22.5°)
        sine_table[3]  = 201; // Sin(33.75°)
        sine_table[4]  = 222; // Sin(45°)
        sine_table[5]  = 239; // Sin(56.25°)
        sine_table[6]  = 252; // Sin(67.5°)
        sine_table[7]  = 255; // Sin(78.75°)
        sine_table[8]  = 252; // Sin(90°)
        sine_table[9]  = 239;
        sine_table[10] = 222;
        sine_table[11] = 201;
        sine_table[12] = 178;
        sine_table[13] = 153;
        sine_table[14] = 128;
        sine_table[15] = 102;
        sine_table[16] = 77;
        sine_table[17] = 54;
        sine_table[18] = 33;
        sine_table[19] = 16;
        sine_table[20] = 3;
        sine_table[21] = 0;
        sine_table[22] = 3;
        sine_table[23] = 16;
        sine_table[24] = 33;
        sine_table[25] = 54;
        sine_table[26] = 77;
        sine_table[27] = 102;
        sine_table[28] = 128;
        sine_table[29] = 153; 
        sine_table[30] = 178;
        sine_table[31] = 201;
    end

//    // Logic for a basic square wave with some frequency!
//    //counter cycle period = T_sine * f_clk 
//    always_ff @(posedge clk_100MHz) begin
//        if (counter < 19'd200000) begin
//            // Turn on SPKL and SPKR for the first half of the period
//            SPKL = 1;
//            SPKR = 1;
//            counter = counter + 1;
//            led  = 1;
//        end else if (counter < 19'd400000) begin
//            // Turn off SPKL and SPKR for the second half of the period
//            led = 0;
//            SPKL = 0;
//            SPKR = 0;
//            counter = counter + 1;
//        end else begin
//            // Reset the counter to repeat the cycle
//            counter = 0;
//        end
//    end
    
    //code for a sampled sine wave (current implementation: 250 Hz)
    logic [31:0] slow_counter = 0;  // Slow counter to divide the clock
    logic [7:0] pwm_counter = 0;       // Counter for PWM generation
//    static int vol = 1;
//    always_comb begin
//        vol = volume; 
//    end
    //slow_counter = 100_MHz clk / (number of entries in wave table * frequency of sine wave)
    // = 100,000,000 / (32 * 250)= 12500 cycles 
    // Volume control scaling factor (integer-based)
    localparam int scale_factor_numerator = 50;  // Scale factor numerator (e.g., 0.5 as 50)
    localparam int scale_factor_denominator = 100; // Scale factor denominator (e.g., 1.0 as 100)
    always_ff @(posedge clk_100MHz) begin
        // Ensure frequency and volume are non-zero
        if (freq == 0 || volume == 0 || lock == 0) begin
            SPKL = 0;
            SPKR = 0;
        end else begin
            // Slow counter logic
            if (slow_counter < (100000000/(freq* 32)) - 1) begin
                slow_counter = slow_counter + 1;
            end else begin
                slow_counter = 0;
                if (index == 31) begin
                    index = 0;
                end else begin
                    index = index + 1;
                end
            end
         end
        // Update the duty cycle based on the scaled sine wave value
        duty_cycle = (sine_table[index] * scale_factor_numerator * volume) / scale_factor_denominator;
        
        // Generate PWM signal for SPKL and SPKR
        if (pwm_counter < duty_cycle) begin
            SPKL = 1;
            SPKR = 1;
        end else begin
            SPKL = 0;
            SPKR = 0;
        end

        // Increment the PWM counter
        pwm_counter = pwm_counter + 1;
    end

endmodule
