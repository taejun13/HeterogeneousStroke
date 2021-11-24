using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.IO.Ports;
using System.Threading;
using System.ComponentModel;

namespace HeterogeneousStroke
{
    public partial class MainWindow
    {
        public void MotorFixButtonClick_Parts(int motorNum)
        {
            int[] powers = { (int)motor1power.Value, (int)motor2power.Value, (int)motor3power.Value, (int)motor4power.Value };
            int[] freqs = { (int)motor1frequency.Value, (int)motor2frequency.Value, (int)motor3frequency.Value, (int)motor4frequency.Value };
            bool[] modulations = { (bool)motor1Modulation.IsChecked , (bool)motor2Modulation.IsChecked , (bool)motor3Modulation.IsChecked , (bool)motor4Modulation.IsChecked };
            string powerStr, freqStr, modulationOnTimeStr, modulationOffTimeStr;

            int power = powers[motorNum - 1];
            if (power >= 100)
                powerStr = motorNum + "a" + power.ToString();
            else if (power >= 10)
                powerStr = motorNum + "a0" + power.ToString();
            else
                powerStr = motorNum + "a00" + power.ToString();

            int freq = freqs[motorNum - 1];
            if (freq >= 100)
                freqStr = motorNum + "f" + freq.ToString();
            else if (freq >= 10)
                freqStr = motorNum + "f0" + freq.ToString();
            else
                freqStr = motorNum + "f00" + freq.ToString();

            if (modulateOnTime >= 100)
                modulationOnTimeStr = motorNum + "br" + modulateOnTime.ToString();
            else if (modulateOnTime >= 10)
                modulationOnTimeStr = motorNum + "br0" + modulateOnTime.ToString();
            else
                modulationOnTimeStr = motorNum + "br00" + modulateOnTime.ToString();

            if (modulateOffTime >= 100)
                modulationOffTimeStr = motorNum + "bs" + modulateOffTime.ToString();
            else if (modulateOffTime >= 10)
                modulationOffTimeStr = motorNum + "bs0" + modulateOffTime.ToString();
            else
                modulationOffTimeStr = motorNum + "bs00" + modulateOffTime.ToString();

            bool modulation = modulations[motorNum - 1];
            serialPort1.WriteLine(powerStr);
            serialPort1.WriteLine(freqStr);
            if (modulation)
            {
                serialPort1.WriteLine(modulationOnTimeStr);
                serialPort1.WriteLine(modulationOffTimeStr);
            }
            else
                serialPort1.WriteLine(motorNum + "bs000");

        }

        private void Motor1Fix_Click(object sender, RoutedEventArgs e)
        {
            MotorFixButtonClick_Parts(1);
        }

        private void Motor2Fix_Click(object sender, RoutedEventArgs e)
        {
            MotorFixButtonClick_Parts(2);
        }

        private void Motor3Fix_Click(object sender, RoutedEventArgs e)
        {
            MotorFixButtonClick_Parts(3);
        }

        private void Motor4Fix_Click(object sender, RoutedEventArgs e)
        {
            MotorFixButtonClick_Parts(4);
        }
    }
}
