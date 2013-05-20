using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Text.RegularExpressions;

using System.IO.Ports;
using System.IO;

namespace MotorolaUserConfigApp
{
    public partial class UserConfigApp : Form
    {

        private FileStream _OutStream;
        private StreamWriter _StreamWriter;

        public UserConfigApp()
        {
            InitializeComponent();
            setupSerial();
            //LogData();
        }

        private void UserConfigApp_FormClosing(object sender, FormClosingEventArgs e)
        {
            if (serialPort1.IsOpen)
                serialPort1.Close();

            if (_StreamWriter != null)
                _StreamWriter.Close();
            
            if (_OutStream != null)
                _OutStream.Close();
        }

        private void sendBTN_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen)
            {
                try
                {
                    serialPort1.Write(makeOutStr());
                }
                catch
                {
                    MessageBox.Show("Serial Write Fail");
                }
            }
            else
                MessageBox.Show("Serial port not open");
        }

        private void setupSerial()
        {
            string[] ports = SerialPort.GetPortNames();
            foreach( string port in ports)
                comPorts.Items.Add(port);
            comPorts.Items.Add("None");
        }

        private string makeOutStr()
        {
            string outMsg;

            outMsg = ssidTB.Text + "," + psswrdTB.Text + ",";
            if(soundCB.Checked)
                outMsg +="1,";
            else
                outMsg +="0,";
            if (vibrationCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            if (levelCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            if (tempCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            if (lightCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            if (waterCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            if (motionCB.Checked)
                outMsg += "1,";
            else
                outMsg += "0,";
            
            return outMsg;
        }

        //Handle data recieved from device
        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
           AddDataRecv(sender);
        }

        private delegate void AddDataRecvDelegate(object item);

        private void AddDataRecv(object item)
        {
            if (this.dataRecvTB.InvokeRequired)
            {
                // This is a worker thread so delegate the task.
                this.dataRecvTB.Invoke(new AddDataRecvDelegate(this.AddDataRecv), item);
            }
            else
            {
                // This is the UI thread so perform the task.
                SerialPort sp = (SerialPort)item;
                string indata = sp.ReadExisting();
                this.dataRecvTB.Text = indata + this.dataRecvTB.Text;

                string[] dataEntries = Regex.Split(indata, "Data");
                for (int i = 0; i < dataEntries.Length; i++)
                {
                    string[] sensorVal = Regex.Split(dataEntries[i], "\r\n");
                    bool sensorValRead = false;
                    for (int j = 0; j < sensorVal.Length; j++)
                    {
                        if (checkSensorValue(sensorVal[j]))
                            sensorValRead = true;
                    }
                    if (sensorValRead)
                        printSensors();
                }
            }
        }

        private Boolean checkSensorValue( string sensorVal)
        {
            string[] sensorKeyVal = sensorVal.Split(':');

            if (sensorKeyVal[0] == "SoundFreq1")
            {
                sound1TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "SoundFreq2")
            {
                sound2TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "SoundFreq3")
            {
                sound3TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "VibrationFreq1")
            {
                vib1TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "VibrationFreq3")
            {
                vib3TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "VibrationFreq3")
            {
                vib3TB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "Level")
            {
                levelTB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "Temp")
            {
                tempTB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "Light")
            {
                lightTB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "Water")
            {
                waterTB.Text = sensorKeyVal[1];
                return true;
            }
            else if (sensorKeyVal[0] == "Motion")
            {
                motionTB.Text = sensorKeyVal[1];
                return true;
            }

            return false;
        }

        private void printSensors()
        {
            if(LogDataCB.Checked)
                _StreamWriter.Write(sound1TB.Text+","+sound2TB.Text+","+sound3TB.Text+","+vib1TB.Text+","+vib2TB.Text+","+vib3TB.Text+","+
                    levelTB.Text+","+tempTB.Text+","+lightTB.Text+","+","+waterTB.Text+","+ motionTB.Text+",\n");
        }

        //Change COM Port
        private void comPorts_SelectedIndexChanged(object sender, EventArgs e)
        {
            //Clear all Text fields
            sound1TB.Clear();
            sound2TB.Clear();
            sound3TB.Clear();
            vib1TB.Clear();
            vib2TB.Clear();
            vib3TB.Clear();
            levelTB.Clear();
            tempTB.Clear();
            lightTB.Clear();
            waterTB.Clear();
            motionTB.Clear();
            dataRecvTB.Clear();

            serialPort1.Close();
            string comSel = (string)comPorts.SelectedItem;
            if (comSel != "None")
            {
                serialPort1.PortName = comSel;
                try
                {
                    serialPort1.Open();
                }
                catch (UnauthorizedAccessException uae)
                {
                    MessageBox.Show("Com port is currently in use");
                    comPorts.SelectedIndex = comPorts.FindStringExact("None");
                }
            }
        }

        private void LogDataCB_CheckedChanged(object sender, EventArgs e)
        {
            if (LogDataCB.Checked == true)
            {
                string filename = DateTime.Today.ToString("MM");
                filename += DateTime.Today.ToString("dd")+ "_";
                filename += DateTime.Now.ToString("HH");
                filename += DateTime.Now.ToString("mm");

                _OutStream = new FileStream("output"+filename+".csv", FileMode.Create, FileAccess.Write);
                _StreamWriter = new StreamWriter(_OutStream) { AutoFlush = true };
                _StreamWriter.Write("SoundFreq1,SoundFreq2,SoundFreq3,VibrationFreq1,VibrationFreq2,VibrationFreq3,Level,Temp,Light,Water,Motion,\n");
            }
            else
            {
                if (_StreamWriter != null)
                    _StreamWriter.Close();
                _OutStream.Close();
            }
        }
    }
}
