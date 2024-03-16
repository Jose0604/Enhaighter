/*
  ==============================================================================

    zolzer.h
    Created: 18 Apr 2021 4:00:17pm
    Author:  José Manuel Arias Arias 

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class zolzer
{
public:
    /*zolzer() :
        b0(1), b1(0), b2(0), a1(0), a2(0), Fs(44100), G(0), V(1), Qfactor(1),
        yn_1(nullptr), yn_2(nullptr), yn_3(nullptr), yn_4(nullptr),
        xn_1(nullptr), xn_2(nullptr), xn_3(nullptr), xn_4(nullptr) {}*/

    zolzer(juce::String Type) :
        b0(1), b1(0), b2(0), a1(0), a2(0), Fs(44100), G(0), V(1), Qfactor(1),
        yn_1(nullptr), yn_2(nullptr),xn_1(nullptr), xn_2(nullptr)
    {
        filterType = Type;
        if (filterType == "lowpass")
            fn = 20e3;
        else if (filterType == "highpass")
            fn = 20;
        else if (filterType == "bell")
            fn = 1e3;
        else if (filterType == "lowshelv")
            fn = 500;
        else if(filterType == "highshelv")
            fn = 2e3;
        DBG(fn);
    }

    ~zolzer() {}

    void updateNumInputChannels(int NumInputChannels) {
        

        if (yn_1 != nullptr)
            delete[] yn_1;
        if (yn_2 != nullptr)
            delete[] yn_2;
        //if (yn_3 != nullptr)
        //    delete[] yn_3;
        //if (yn_4 != nullptr)
        //    delete[] yn_4;
        if (xn_1 != nullptr)
            delete[] xn_1;
        if (xn_2 != nullptr)
            delete[] xn_2;
        //if (xn_3 != nullptr)
        //    delete[] xn_3;
        //if (xn_4 != nullptr)
        //    delete[] xn_4;

        yn_1 = new double[NumInputChannels];
        yn_2 = new double[NumInputChannels];
        //yn_3 = new double[NumInputChannels];
        //yn_4 = new double[NumInputChannels];

        xn_1 = new double[NumInputChannels];
        xn_2 = new double[NumInputChannels];
        //xn_3 = new double[NumInputChannels];
        //xn_4 = new double[NumInputChannels];

        for (int channel = 0; channel < NumInputChannels; ++channel)
        {
            yn_1[channel] = 0;
            yn_2[channel] = 0;
            /*yn_3[channel] = 0;
            yn_4[channel] = 0;*/

            xn_1[channel] = 0;
            xn_2[channel] = 0;
            /*xn_3[channel] = 0;
            xn_4[channel] = 0;*/
        }
    }

    void updateSampleRate(int sR)
    {
        Fs = sR;
        coefficients();
    }

    void updateFilterType(juce::String Type)
    {
        filterType = Type;
        coefficients();
    }

    void updateCutFrequency(double f)
    {
        fn = f;
        coefficients();
    }

    void updateQfactor(double q)
    {
        Qfactor = q;
        coefficients();
    }

    void updateGdB(double dB)
    {
        G = dB;
        coefficients();
    }

    void coefficients()
    {
        double Q =  (Qfactor / raizDe2);
        double K = tan(juce::MathConstants<double>::pi * fn / Fs);
        if (filterType == "lowpass")
        {
            b0 = (pow(K, 2) * Q) / (pow(K, 2) * Q + K + Q);
            b1 = (2 * pow(K, 2) * Q) / (pow(K, 2) * Q + K + Q);
            b2 = b0;

            a1 = 2 *  Q * (pow(K, 2) - 1) / (pow(K, 2) * Q + K + Q);
            a2 = (pow(K, 2) * Q - K + Q) / (pow(K, 2) * Q + K + Q);
        }
        else if (filterType == "highpass")
        {
            b0 = Q / (pow(K, 2) * Q + K + Q);
            b1 = -2 *  Q / (pow(K, 2) * Q + K + Q);
            b2 = Q / (pow(K, 2) * Q + K + Q);

            a1 = 2 *  Q * (pow(K, 2) - 1) / (pow(K, 2) * Q + K + Q);
            a2 = (pow(K, 2) * Q - K + Q) / (pow(K, 2) * Q + K + Q);
        }
        else if (filterType == "lowshelv")
        {
            V = pow(10, (G / 20));
            if (G > 0)
            {
                b0 = (1 + sqrt( 2 * V) * K + V * pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
                b1 = 2 * (V * pow(K, 2) - 1) / (1 + sqrt(2) * K + pow(K, 2));
                b2 = (1 - sqrt( 2 * V) * K + V * pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));

                a1 = 2 * (pow(K, 2) - 1) / (1 + sqrt(2) * K + pow(K, 2));
                a2 = (1 - sqrt(2) * K + pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));

            }
            else
            {
                b0 = V * (1 + sqrt(2) * K + pow(K, 2)) / (V + sqrt(2 * V) * K + pow(K, 2));
                b1 = 2 * V * (pow(K, 2) - 1) / (V + sqrt(2 * V) * K + pow(K, 2));
                b2 = V * (1 - sqrt(2) * K + pow(K, 2)) / (V + sqrt(2 * V) * K + pow(K, 2));

                a1 = 2 * (pow(K, 2) - V) / (V + sqrt(2 * V) * K + pow(K, 2));
                a2 = (V - sqrt(2 * V) * K + pow(K, 2)) / (V + sqrt(2 * V) * K + pow(K, 2));
            }
            //DBG(a2);
        }
        else if (filterType == "highshelv")
        {

            V = pow(10, (G / 20));
            if (G > 0)
            {
                b0 = (V + sqrt( 2 * V) * K + pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
                b1 = 2 * (pow(K, 2) - V) / (1 + sqrt(2) * K + pow(K, 2));
                b2 = (V - sqrt( 2 * V) * K + pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));

                a1 = 2 * (pow(K, 2) - 1) / (1 + sqrt(2) * K + pow(K, 2));
                a2 = (1 - sqrt(2) * K + pow(K, 2)) / (1 + sqrt(2) * K + pow(K, 2));
            }
            else
            {
                b0 = V * (1 + sqrt(2) * K + pow(K, 2)) / (1 + sqrt( 2 * V) * K + V * pow(K, 2));
                b1 = 2 *  V * (pow(K, 2) - 1) / (1 + sqrt( 2 * V) * K + V * pow(K, 2));
                b2 = V * (1 - sqrt(2) * K + pow(K, 2)) / (1 + sqrt( 2 * V) * K + V * pow(K, 2));

                a1 = 2 * (V * pow(K, 2) - 1) / (1 + sqrt( 2 * V) * K + V * pow(K, 2));
                a2 = (1 - sqrt( 2 * V) * K + V * pow(K, 2)) / (1 + sqrt( 2 * V) * K + V * pow(K, 2));
            }
            //DBG(a2);
        }
        else if (filterType == "bell")
        {

            V = pow(10, (G / 20));
            if (G > 0)
            {
                b0 = (1 +  V * K / Q + pow(K, 2)) / (1 +  K / Q + pow(K, 2));
                b1 = 2 * (pow(K, 2) - 1) / (1 +  K / Q + pow(K, 2));
                b2 = (1 -  V * K / Q + pow(K, 2)) / (1 +  K / Q + pow(K, 2));

                a1 = 2 * (pow(K, 2) - 1) / (1 +  K / Q + pow(K, 2));
                a2 = (1 -  K / Q + pow(K, 2)) / (1 +  K / Q + pow(K, 2));
            }
            else
            {
                b0 = (1 +  K / Q + pow(K, 2)) / (1 +  K / (Q * V) + pow(K, 2));
                b1 = 2 * (pow(K, 2) - 1) / (1 +  K / (Q * V) + pow(K, 2));
                b2 = (1 -  K / Q + pow(K, 2)) / (1 +  K / (Q * V) + pow(K, 2));

                a1 = 2 * (pow(K, 2) - 1) / (1 +  K / (Q * V) + pow(K, 2));
                a2 = (1 -  K / (Q * V) + pow(K, 2)) / (1 +  K / (Q * V) + pow(K, 2));
            }
            
        }
    }

    float filter(double xn,int channel, bool isFilterOn)
    {
        double yn;
        if (isFilterOn)
        {
            yn = b0 * xn + b1 * xn_1[channel] + b2 * xn_2[channel] - a1 * yn_1[channel] - a2 * yn_2[channel];
            //if (!isActive4Order)
            //{
            //    yn = b0 * xn + b1 * xn_1[channel] + b2 * xn_2[channel] - a1 * yn_1[channel] - a2 * yn_2[channel];
            //    //xn_2[channel] = xn_1[channel];
            //    //xn_1[channel] = xn;
            //    //yn_2[channel] = yn_1[channel];
            //    //yn_1[channel] = yn;
            //}
            //else if (isActive4Order)
            //{
            //    yn = pow(b0, 2) * xn + 2 * b1 * b0 * xn_1[channel] + (2 * b2 * b0 + pow(b1, 2)) * xn_2[channel] +
            //        2 * b2 * b1 * xn_3[channel] + pow(b2, 2) * xn_4[channel] - (2 * a1 * yn_1[channel] +
            //            (2 * a2 + pow(a1, 2)) * yn_2[channel] + 2 * a2 * a1 * yn_3[channel] + pow(a2, 2) * yn_4[channel]);

            //    /*xn_4[channel] = xn_3[channel];
            //    xn_3[channel] = xn_2[channel];
            //    xn_2[channel] = xn_1[channel];
            //    xn_1[channel] = xn;

            //    yn_4[channel] = yn_3[channel];
            //    yn_3[channel] = yn_2[channel];
            //    yn_2[channel] = yn_1[channel];
            //    yn_1[channel] = yn;*/
            //}
            /*xn_4[channel] = xn_3[channel];
            xn_3[channel] = xn_2[channel];*/
            xn_2[channel] = xn_1[channel];
            xn_1[channel] = xn;

            /*yn_4[channel] = yn_3[channel];
            yn_3[channel] = yn_2[channel];*/
            yn_2[channel] = yn_1[channel];
            yn_1[channel] = yn;
        }
        else
        {
            yn = xn;
        }
        return (float) yn;
    }

private:

    /*double b0, b1, b2, a1, a2, V, Qfactor, fn, G,
        *yn_1, *yn_2, *yn_3, *yn_4,
        *xn_1, *xn_2, *xn_3, *xn_4;*/
    double b0, b1, b2, a1, a2, V, Qfactor, fn, G,
        * yn_1, * yn_2, * xn_1, * xn_2;
    const double raizDe2 = juce::MathConstants<double>::sqrt2;

    int Fs;

    juce::String filterType;
};