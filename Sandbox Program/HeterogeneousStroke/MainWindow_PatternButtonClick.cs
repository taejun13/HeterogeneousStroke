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
        string[] patternSet = { "1243", "1342", "1234", "2143",
                                "134", "243", "132", "124", "312", "213", "234", "324",
                                "12", "13", "14", "21", "23", "24", "31", "32", "34", "41", "42", "43",
                                "1", "2", "3", "4"};
        string currentAskingPattern = "";
        bool patternAnswering = false;

        private void randomPlay_Click(object sender, RoutedEventArgs e)
        {
            if (!patternAnswering)
            {
                randomPlayAnswerLabel.Visibility = Visibility.Hidden;
                Random rnd = new Random();
                patternSet = patternSet.OrderBy(x => rnd.Next()).ToArray();

                currentAskingPattern = patternSet[0];
                patternGenerate(patternSet[0]);
                patternAnswering = true;
            }
        }

        public void patternButtonClick_Parts(string str)
        {
            if (patternAnswering)
            {
                if (currentAskingPattern == str)
                    randomPlayAnswerLabel.Content = "Correct";
                else
                    randomPlayAnswerLabel.Content = "Wrong,\n Actual\n Pattern\n: " + currentAskingPattern;
                randomPlayAnswerLabel.Visibility = Visibility.Visible;
                patternAnswering = false;
            }
            else
                patternGenerate(str);
        }

        private void Pattern1_Click(object sender, RoutedEventArgs e)
        {
            string str = "1243";
            patternButtonClick_Parts(str);
        }
        private void Pattern2_Click(object sender, RoutedEventArgs e)
        {
            string str = "1342";
            patternButtonClick_Parts(str);
        }
        private void Pattern3_Click(object sender, RoutedEventArgs e)
        {
            string str = "1234";
            patternButtonClick_Parts(str);
        }
        private void Pattern4_Click(object sender, RoutedEventArgs e)
        {
            string str = "2143";
            patternButtonClick_Parts(str);
        }
        private void Pattern5_Click(object sender, RoutedEventArgs e)
        {
            string str = "134";
            patternButtonClick_Parts(str);
        }
        private void Pattern6_Click(object sender, RoutedEventArgs e)
        {
            string str = "243";
            patternButtonClick_Parts(str);
        }
        private void Pattern7_Click(object sender, RoutedEventArgs e)
        {
            string str = "132";
            patternButtonClick_Parts(str);
        }
        private void Pattern8_Click(object sender, RoutedEventArgs e)
        {
            string str = "124";
            patternButtonClick_Parts(str);
        }
        private void Pattern9_Click(object sender, RoutedEventArgs e)
        {
            string str = "312";
            patternButtonClick_Parts(str);
        }
        private void Pattern10_Click(object sender, RoutedEventArgs e)
        {
            string str = "213";
            patternButtonClick_Parts(str);
        }
        private void Pattern11_Click(object sender, RoutedEventArgs e)
        {
            string str = "234";
            patternButtonClick_Parts(str);
        }
        private void Pattern12_Click(object sender, RoutedEventArgs e)
        {
            string str = "324";
            patternButtonClick_Parts(str);
        }
        private void Pattern13_Click(object sender, RoutedEventArgs e)
        {
            string str = "23";
            patternButtonClick_Parts(str);
        }
        private void Pattern14_Click(object sender, RoutedEventArgs e)
        {
            string str = "21";
            patternButtonClick_Parts(str);
        }
        private void Pattern15_Click(object sender, RoutedEventArgs e)
        {
            string str = "31";
            patternButtonClick_Parts(str);
        }
        private void Pattern16_Click(object sender, RoutedEventArgs e)
        {
            string str = "32";
            patternButtonClick_Parts(str);
        }
        private void Pattern17_Click(object sender, RoutedEventArgs e)
        {
            string str = "34";
            patternButtonClick_Parts(str);
        }
        private void Pattern18_Click(object sender, RoutedEventArgs e)
        {
            string str = "43";
            patternButtonClick_Parts(str);
        }
        private void Pattern19_Click(object sender, RoutedEventArgs e)
        {
            string str = "41";
            patternButtonClick_Parts(str);
        }
        private void Pattern20_Click(object sender, RoutedEventArgs e)
        {
            string str = "42";
            patternButtonClick_Parts(str);
        }
        private void Pattern21_Click(object sender, RoutedEventArgs e)
        {
            string str = "12";
            patternButtonClick_Parts(str);
        }
        private void Pattern22_Click(object sender, RoutedEventArgs e)
        {
            string str = "14";
            patternButtonClick_Parts(str);
        }
        private void Pattern23_Click(object sender, RoutedEventArgs e)
        {
            string str = "13";
            patternButtonClick_Parts(str);
        }
        private void Pattern24_Click(object sender, RoutedEventArgs e)
        {
            string str = "24";
            patternButtonClick_Parts(str);
        }
        private void Pattern25_Click(object sender, RoutedEventArgs e)
        {
            string str = "1";
            patternButtonClick_Parts(str);
        }
        private void Pattern26_Click(object sender, RoutedEventArgs e)
        {
            string str = "2";
            patternButtonClick_Parts(str);
        }
        private void Pattern27_Click(object sender, RoutedEventArgs e)
        {
            string str = "3";
            patternButtonClick_Parts(str);
        }
        private void Pattern28_Click(object sender, RoutedEventArgs e)
        {
            string str = "4";
            patternButtonClick_Parts(str);
        }
    }
}
