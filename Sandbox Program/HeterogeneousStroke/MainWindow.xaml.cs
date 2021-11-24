using System;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.IO.Ports;
using System.Threading;
using System.ComponentModel;

namespace HeterogeneousStroke
{
    /*
     * Writer: Taejun Kim, HCI Lab KAIST - https://taejun13.github.io/
     * Last Update: 2021. 11. 24
     * Heterogeneous Stroke: Using Unique Vibration Cues to Improve the Wrist-Worn Spatiotemporal Tactile Display
     * ACM CHI 21': Conference on Human Factors in Computing Systems.
     * DOI: https://dl.acm.org/doi/abs/10.1145/3411764.3445448
     */
    public partial class MainWindow : Window
    {
        SerialPort serialPort1 = new SerialPort();

        int unitVibratingDuration = 500;
        int modulateOnTime = 40;
        int modulateOffTime = 40;

        public MainWindow()
        {
            InitializeComponent();
        }

        /* For Connecting Arduino Board */
        private void ButtonReset_Click(object sender, RoutedEventArgs e)
        {
            string[] ports = SerialPort.GetPortNames();
            ComboboxSerials.Items.Clear();

            for (int i = 0; i < ports.Length; i++)
                ComboboxSerials.Items.Add(ports[i]);
            if (ports.Length > 0)
            {
                ComboboxSerials.SelectedIndex = ComboboxSerials.Items.Count - 1;
                serialPort1.BaudRate = 115200;
                serialPort1.DtrEnable = true;
                serialPort1.RtsEnable = true;
            }
        }

        /* For Connecting Arduino Board */
        private void ButtonConnect_Click(object sender, RoutedEventArgs e)
        {
            if (!serialPort1.IsOpen)
            {
                serialPort1.PortName = (string)ComboboxSerials.Items[ComboboxSerials.SelectedIndex];
                serialPort1.Open();
                string line = serialPort1.ReadExisting();
                Console.WriteLine("Start");
            }
            else
            {
                serialPort1.Close();
            }
        }

        /* Send commands to arduino board to control the actuator (Use pre-defined protocol - e.g., 1v, 2s, 1f300, 2a030, 4br040) */
        public void stimulation(int tactorNum)
        {
            serialPort1.WriteLine(tactorNum.ToString() + "v");
            Thread.Sleep(unitVibratingDuration);
            serialPort1.WriteLine(tactorNum.ToString() + "s");
        }


        // Input: String (ex. "1243")
        // Generates spatio-temporal tactile patterns
        public void patternGenerate(String text)
        {
            int n = text.Length;
            for (int i = 0; i < n; i++)
                stimulation(Convert.ToInt32(text[i].ToString()));
        }

        private void vibDurationFix_Click(object sender, RoutedEventArgs e)
        {
            unitVibratingDuration = (int)vibDuration.Value;
        }

        private void modulationOnTimeFix_Click(object sender, RoutedEventArgs e)
        {
            modulateOnTime = (int)modulationOnTime.Value;
            string modulateOnTimeStr, modulateOffTimeStr;
            for (int i = 0; i < 4; i++)
            {
                CheckBox[] modulations = { motor1Modulation, motor2Modulation, motor3Modulation, motor4Modulation };
                if (modulateOnTime >= 100)
                    modulateOnTimeStr = (i+1).ToString() + "br" + modulateOnTime.ToString();
                else if (modulateOnTime >= 10)
                    modulateOnTimeStr = (i + 1).ToString() + "br0" + modulateOnTime.ToString();
                else
                    modulateOnTimeStr = (i + 1).ToString() + "br00" + modulateOnTime.ToString();

                if (modulateOffTime >= 100)
                    modulateOffTimeStr = (i + 1).ToString() + "bs" + modulateOffTime.ToString();
                else if (modulateOffTime >= 10)
                    modulateOffTimeStr = (i + 1).ToString() + "bs0" + modulateOffTime.ToString();
                else
                    modulateOffTimeStr = (i + 1).ToString() + "bs00" + modulateOffTime.ToString();

                bool modulation = (bool)modulations[i].IsChecked;
                if (modulation)
                {
                    serialPort1.WriteLine(modulateOnTimeStr);
                    serialPort1.WriteLine(modulateOffTimeStr);
                }
                else
                {
                    serialPort1.WriteLine((i + 1).ToString() + "bs000");
                }
            }
            
        }

        private void modulationOffTimeFix_Click(object sender, RoutedEventArgs e)
        {
            modulateOffTime = (int)modulationOffTime.Value;
            string modulateOnTimeStr, modulateOffTimeStr;
            for (int i = 0; i < 4; i++)
            {
                CheckBox[] modulations = { motor1Modulation, motor2Modulation, motor3Modulation, motor4Modulation };
                if (modulateOnTime >= 100)
                    modulateOnTimeStr = (i + 1).ToString() + "br" + modulateOnTime.ToString();
                else if (modulateOnTime >= 10)
                    modulateOnTimeStr = (i + 1).ToString() + "br0" + modulateOnTime.ToString();
                else
                    modulateOnTimeStr = (i + 1).ToString() + "br00" + modulateOnTime.ToString();

                if (modulateOffTime >= 100)
                    modulateOffTimeStr = (i + 1).ToString() + "bs" + modulateOffTime.ToString();
                else if (modulateOffTime >= 10)
                    modulateOffTimeStr = (i + 1).ToString() + "bs0" + modulateOffTime.ToString();
                else
                    modulateOffTimeStr = (i + 1).ToString() + "bs00" + modulateOffTime.ToString();

                bool modulation = (bool)modulations[i].IsChecked;
                if (modulation)
                {
                    serialPort1.WriteLine(modulateOnTimeStr);
                    serialPort1.WriteLine(modulateOffTimeStr);
                }
                else
                {
                    serialPort1.WriteLine((i + 1).ToString() + "bs000");
                }
            }
        }
    }
}
