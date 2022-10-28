// Copyright 2022 GRAME

#include <iostream>
#include <iomanip>

#include "ysfx.h"

using namespace std;

int main(int argc, char* argv[])
{
    ysfx_config_u config{ysfx_config_new()};
    ysfx_u fx{ysfx_new(config.get())};
    
    if (!ysfx_load_file(fx.get(), argv[1], 0)) {
        cout << "File not found:" << argv[1] << endl;
        return -1;
    }
    
    if (!ysfx_compile(fx.get(), 0)) {
        cout << "Cannot compile:" << argv[1] << endl;
        return -1;
    }
    
    int i = 0;
    while (ysfx_slider_exists(fx.get(), i)) {
        cout << "name: " << ysfx_slider_get_name(fx.get(), i) << endl;
        cout << "value: " << ysfx_slider_get_value(fx.get(), i) << endl;
        ysfx_slider_range_t range;
        if (ysfx_slider_get_range(fx.get(), i, &range)) {
            cout << "def: " << range.def << endl;
            cout << "min: " << range.min << endl;
            cout << "max: " << range.max << endl;
            cout << "inc: " << range.inc << endl;
        }
        i++;
    }
   
    ysfx_set_sample_rate(fx.get(), 44100.0);
    
    ysfx_init(fx.get());
    
    int inputs = ysfx_get_num_inputs(fx.get());
    int outputs = ysfx_get_num_outputs(fx.get());
    
    int SIZE = 128;
    
    cout << "inputs: " <<  inputs << endl;
    for (int i = 0; i < inputs; i++) {
        cout << "input: " << ysfx_get_input_name(fx.get(), i) << endl;
    }
    cout << "outputs: " << outputs << endl;
    for (int i = 0; i < outputs ; i++) {
        cout << "output: " << ysfx_get_output_name(fx.get(), i) << endl;
    }
    
    double** inputsBuf = new double*[inputs];
    double** outputBuf = new double*[outputs];
    
    for (int i = 0; i < inputs; i++) {
        inputsBuf[i] = new double[SIZE];
    }
    for (int i = 0; i < outputs; i++) {
        outputBuf[i] = new double[SIZE];
    }
    
    ysfx_process_double(fx.get(), (const double **)inputsBuf, (double **)outputBuf, inputs, outputs, SIZE);
    
    for (int chan = 0; chan < outputs; chan++) {
        for (int frame = 0; frame < SIZE; frame++) {
            cout << setprecision(std::numeric_limits<float>::max_digits10 + 2) << fixed << "chan: " << chan << " frame: " << frame << " " << outputBuf[chan][frame] << endl;
        }
    }
    
    return 0;
}
