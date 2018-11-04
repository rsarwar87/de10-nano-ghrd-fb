module user_input_device (
    input clk,
    input reset,
    input [1:0] keys,
    input [3:0] switches,
    input [25:0] data,

    output avl_irq,
    input  avl_read,
    output [31:0] avl_readdata
);

reg [31:0] cur_inputs;
reg [31:0] last_inputs;
wire [31:0] changed_inputs = cur_inputs ^ last_inputs;

reg irq;

assign avl_irq = irq;
assign avl_readdata = last_inputs;

always @(posedge clk) begin
    if (reset) begin
        cur_inputs <= 32'd0;
        last_inputs <= 32'd0;
        irq <= 1'b0;
    end else begin
        cur_inputs <= {keys, switches};
        last_inputs <= cur_inputs;
        if (changed_inputs != 32'd0)
            irq <= 1'b1;
        else if (avl_read)
            irq <= 1'b0;
    end
end

endmodule