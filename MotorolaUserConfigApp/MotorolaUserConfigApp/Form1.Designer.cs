namespace MotorolaUserConfigApp
{
    partial class UserConfigApp
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.button1 = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.comPorts = new System.Windows.Forms.ComboBox();
            this.comPortLBL = new System.Windows.Forms.Label();
            this.psswrdLBL = new System.Windows.Forms.Label();
            this.ssidLBL = new System.Windows.Forms.Label();
            this.psswrdTB = new System.Windows.Forms.TextBox();
            this.ssidTB = new System.Windows.Forms.TextBox();
            this.sensorsGRP = new System.Windows.Forms.GroupBox();
            this.vib3TB = new System.Windows.Forms.TextBox();
            this.sound3TB = new System.Windows.Forms.TextBox();
            this.vib2TB = new System.Windows.Forms.TextBox();
            this.sound2TB = new System.Windows.Forms.TextBox();
            this.vib1TB = new System.Windows.Forms.TextBox();
            this.sound1TB = new System.Windows.Forms.TextBox();
            this.levelTB = new System.Windows.Forms.TextBox();
            this.lightTB = new System.Windows.Forms.TextBox();
            this.tempTB = new System.Windows.Forms.TextBox();
            this.motionTB = new System.Windows.Forms.TextBox();
            this.waterTB = new System.Windows.Forms.TextBox();
            this.motionCB = new System.Windows.Forms.CheckBox();
            this.waterCB = new System.Windows.Forms.CheckBox();
            this.lightCB = new System.Windows.Forms.CheckBox();
            this.tempCB = new System.Windows.Forms.CheckBox();
            this.levelCB = new System.Windows.Forms.CheckBox();
            this.vibrationCB = new System.Windows.Forms.CheckBox();
            this.soundCB = new System.Windows.Forms.CheckBox();
            this.networkGRP = new System.Windows.Forms.GroupBox();
            this.dataRecvTB = new System.Windows.Forms.TextBox();
            this.LogDataCB = new System.Windows.Forms.CheckBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.sensorsGRP.SuspendLayout();
            this.networkGRP.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(6, 125);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Send";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.sendBTN_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 57600;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // comPorts
            // 
            this.comPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comPorts.FormattingEnabled = true;
            this.comPorts.Location = new System.Drawing.Point(73, 446);
            this.comPorts.Name = "comPorts";
            this.comPorts.Size = new System.Drawing.Size(57, 21);
            this.comPorts.TabIndex = 1;
            this.comPorts.SelectedIndexChanged += new System.EventHandler(this.comPorts_SelectedIndexChanged);
            // 
            // comPortLBL
            // 
            this.comPortLBL.AutoSize = true;
            this.comPortLBL.Location = new System.Drawing.Point(9, 449);
            this.comPortLBL.Name = "comPortLBL";
            this.comPortLBL.Size = new System.Drawing.Size(53, 13);
            this.comPortLBL.TabIndex = 2;
            this.comPortLBL.Text = "COM Port";
            // 
            // psswrdLBL
            // 
            this.psswrdLBL.AutoSize = true;
            this.psswrdLBL.Location = new System.Drawing.Point(6, 44);
            this.psswrdLBL.Name = "psswrdLBL";
            this.psswrdLBL.Size = new System.Drawing.Size(53, 13);
            this.psswrdLBL.TabIndex = 3;
            this.psswrdLBL.Text = "Password";
            // 
            // ssidLBL
            // 
            this.ssidLBL.AutoSize = true;
            this.ssidLBL.Location = new System.Drawing.Point(6, 68);
            this.ssidLBL.Name = "ssidLBL";
            this.ssidLBL.Size = new System.Drawing.Size(32, 13);
            this.ssidLBL.TabIndex = 4;
            this.ssidLBL.Text = "SSID";
            // 
            // psswrdTB
            // 
            this.psswrdTB.Location = new System.Drawing.Point(70, 41);
            this.psswrdTB.Name = "psswrdTB";
            this.psswrdTB.Size = new System.Drawing.Size(89, 20);
            this.psswrdTB.TabIndex = 5;
            // 
            // ssidTB
            // 
            this.ssidTB.Location = new System.Drawing.Point(70, 64);
            this.ssidTB.Name = "ssidTB";
            this.ssidTB.Size = new System.Drawing.Size(100, 20);
            this.ssidTB.TabIndex = 6;
            // 
            // sensorsGRP
            // 
            this.sensorsGRP.Controls.Add(this.vib3TB);
            this.sensorsGRP.Controls.Add(this.sound3TB);
            this.sensorsGRP.Controls.Add(this.vib2TB);
            this.sensorsGRP.Controls.Add(this.sound2TB);
            this.sensorsGRP.Controls.Add(this.vib1TB);
            this.sensorsGRP.Controls.Add(this.sound1TB);
            this.sensorsGRP.Controls.Add(this.levelTB);
            this.sensorsGRP.Controls.Add(this.lightTB);
            this.sensorsGRP.Controls.Add(this.tempTB);
            this.sensorsGRP.Controls.Add(this.motionTB);
            this.sensorsGRP.Controls.Add(this.waterTB);
            this.sensorsGRP.Controls.Add(this.motionCB);
            this.sensorsGRP.Controls.Add(this.waterCB);
            this.sensorsGRP.Controls.Add(this.lightCB);
            this.sensorsGRP.Controls.Add(this.tempCB);
            this.sensorsGRP.Controls.Add(this.levelCB);
            this.sensorsGRP.Controls.Add(this.vibrationCB);
            this.sensorsGRP.Controls.Add(this.soundCB);
            this.sensorsGRP.Location = new System.Drawing.Point(6, 6);
            this.sensorsGRP.Name = "sensorsGRP";
            this.sensorsGRP.Size = new System.Drawing.Size(422, 185);
            this.sensorsGRP.TabIndex = 7;
            this.sensorsGRP.TabStop = false;
            this.sensorsGRP.Text = "Sensors";
            // 
            // vib3TB
            // 
            this.vib3TB.Location = new System.Drawing.Point(312, 41);
            this.vib3TB.Name = "vib3TB";
            this.vib3TB.Size = new System.Drawing.Size(100, 20);
            this.vib3TB.TabIndex = 17;
            // 
            // sound3TB
            // 
            this.sound3TB.Location = new System.Drawing.Point(312, 18);
            this.sound3TB.Name = "sound3TB";
            this.sound3TB.Size = new System.Drawing.Size(100, 20);
            this.sound3TB.TabIndex = 16;
            // 
            // vib2TB
            // 
            this.vib2TB.Location = new System.Drawing.Point(206, 40);
            this.vib2TB.Name = "vib2TB";
            this.vib2TB.Size = new System.Drawing.Size(100, 20);
            this.vib2TB.TabIndex = 15;
            // 
            // sound2TB
            // 
            this.sound2TB.Location = new System.Drawing.Point(206, 17);
            this.sound2TB.Name = "sound2TB";
            this.sound2TB.Size = new System.Drawing.Size(100, 20);
            this.sound2TB.TabIndex = 14;
            // 
            // vib1TB
            // 
            this.vib1TB.Location = new System.Drawing.Point(100, 41);
            this.vib1TB.Name = "vib1TB";
            this.vib1TB.Size = new System.Drawing.Size(100, 20);
            this.vib1TB.TabIndex = 13;
            // 
            // sound1TB
            // 
            this.sound1TB.Location = new System.Drawing.Point(100, 18);
            this.sound1TB.Name = "sound1TB";
            this.sound1TB.Size = new System.Drawing.Size(100, 20);
            this.sound1TB.TabIndex = 12;
            // 
            // levelTB
            // 
            this.levelTB.Location = new System.Drawing.Point(100, 64);
            this.levelTB.Name = "levelTB";
            this.levelTB.Size = new System.Drawing.Size(100, 20);
            this.levelTB.TabIndex = 11;
            // 
            // lightTB
            // 
            this.lightTB.Location = new System.Drawing.Point(100, 111);
            this.lightTB.Name = "lightTB";
            this.lightTB.Size = new System.Drawing.Size(100, 20);
            this.lightTB.TabIndex = 10;
            // 
            // tempTB
            // 
            this.tempTB.Location = new System.Drawing.Point(100, 88);
            this.tempTB.Name = "tempTB";
            this.tempTB.Size = new System.Drawing.Size(100, 20);
            this.tempTB.TabIndex = 9;
            // 
            // motionTB
            // 
            this.motionTB.Location = new System.Drawing.Point(100, 157);
            this.motionTB.Name = "motionTB";
            this.motionTB.Size = new System.Drawing.Size(100, 20);
            this.motionTB.TabIndex = 8;
            // 
            // waterTB
            // 
            this.waterTB.Location = new System.Drawing.Point(100, 134);
            this.waterTB.Name = "waterTB";
            this.waterTB.Size = new System.Drawing.Size(100, 20);
            this.waterTB.TabIndex = 7;
            // 
            // motionCB
            // 
            this.motionCB.AutoSize = true;
            this.motionCB.Location = new System.Drawing.Point(6, 159);
            this.motionCB.Name = "motionCB";
            this.motionCB.Size = new System.Drawing.Size(58, 17);
            this.motionCB.TabIndex = 6;
            this.motionCB.Text = "Motion";
            this.motionCB.UseVisualStyleBackColor = true;
            // 
            // waterCB
            // 
            this.waterCB.AutoSize = true;
            this.waterCB.Location = new System.Drawing.Point(6, 136);
            this.waterCB.Name = "waterCB";
            this.waterCB.Size = new System.Drawing.Size(88, 17);
            this.waterCB.TabIndex = 5;
            this.waterCB.Text = "Water Sense";
            this.waterCB.UseVisualStyleBackColor = true;
            // 
            // lightCB
            // 
            this.lightCB.AutoSize = true;
            this.lightCB.Location = new System.Drawing.Point(6, 113);
            this.lightCB.Name = "lightCB";
            this.lightCB.Size = new System.Drawing.Size(49, 17);
            this.lightCB.TabIndex = 4;
            this.lightCB.Text = "Light";
            this.lightCB.UseVisualStyleBackColor = true;
            // 
            // tempCB
            // 
            this.tempCB.AutoSize = true;
            this.tempCB.Location = new System.Drawing.Point(6, 90);
            this.tempCB.Name = "tempCB";
            this.tempCB.Size = new System.Drawing.Size(86, 17);
            this.tempCB.TabIndex = 3;
            this.tempCB.Text = "Tempurature";
            this.tempCB.UseVisualStyleBackColor = true;
            // 
            // levelCB
            // 
            this.levelCB.AutoSize = true;
            this.levelCB.Location = new System.Drawing.Point(6, 67);
            this.levelCB.Name = "levelCB";
            this.levelCB.Size = new System.Drawing.Size(52, 17);
            this.levelCB.TabIndex = 2;
            this.levelCB.Text = "Level";
            this.levelCB.UseVisualStyleBackColor = true;
            // 
            // vibrationCB
            // 
            this.vibrationCB.AutoSize = true;
            this.vibrationCB.Location = new System.Drawing.Point(6, 43);
            this.vibrationCB.Name = "vibrationCB";
            this.vibrationCB.Size = new System.Drawing.Size(67, 17);
            this.vibrationCB.TabIndex = 1;
            this.vibrationCB.Text = "Vibration";
            this.vibrationCB.UseVisualStyleBackColor = true;
            // 
            // soundCB
            // 
            this.soundCB.AutoSize = true;
            this.soundCB.Location = new System.Drawing.Point(6, 20);
            this.soundCB.Name = "soundCB";
            this.soundCB.Size = new System.Drawing.Size(57, 17);
            this.soundCB.TabIndex = 0;
            this.soundCB.Text = "Sound";
            this.soundCB.UseVisualStyleBackColor = true;
            // 
            // networkGRP
            // 
            this.networkGRP.Controls.Add(this.psswrdTB);
            this.networkGRP.Controls.Add(this.ssidTB);
            this.networkGRP.Controls.Add(this.psswrdLBL);
            this.networkGRP.Controls.Add(this.ssidLBL);
            this.networkGRP.Location = new System.Drawing.Point(6, 6);
            this.networkGRP.Name = "networkGRP";
            this.networkGRP.Size = new System.Drawing.Size(220, 90);
            this.networkGRP.TabIndex = 8;
            this.networkGRP.TabStop = false;
            this.networkGRP.Text = "Network Info";
            // 
            // dataRecvTB
            // 
            this.dataRecvTB.Location = new System.Drawing.Point(3, 3);
            this.dataRecvTB.Multiline = true;
            this.dataRecvTB.Name = "dataRecvTB";
            this.dataRecvTB.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataRecvTB.Size = new System.Drawing.Size(428, 400);
            this.dataRecvTB.TabIndex = 9;
            // 
            // LogDataCB
            // 
            this.LogDataCB.AutoSize = true;
            this.LogDataCB.Location = new System.Drawing.Point(12, 197);
            this.LogDataCB.Name = "LogDataCB";
            this.LogDataCB.Size = new System.Drawing.Size(70, 17);
            this.LogDataCB.TabIndex = 10;
            this.LogDataCB.Text = "Log Data";
            this.LogDataCB.UseVisualStyleBackColor = true;
            this.LogDataCB.CheckedChanged += new System.EventHandler(this.LogDataCB_CheckedChanged);
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Location = new System.Drawing.Point(12, 0);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(442, 440);
            this.tabControl1.TabIndex = 11;
            // 
            // tabPage1
            // 
            this.tabPage1.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage1.Controls.Add(this.sensorsGRP);
            this.tabPage1.Controls.Add(this.LogDataCB);
            this.tabPage1.Location = new System.Drawing.Point(4, 22);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(434, 414);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "Sensor";
            // 
            // tabPage2
            // 
            this.tabPage2.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage2.Controls.Add(this.networkGRP);
            this.tabPage2.Controls.Add(this.button1);
            this.tabPage2.Location = new System.Drawing.Point(4, 22);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(434, 414);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "Wifi Info";
            // 
            // tabPage3
            // 
            this.tabPage3.BackColor = System.Drawing.SystemColors.Control;
            this.tabPage3.Controls.Add(this.dataRecvTB);
            this.tabPage3.Location = new System.Drawing.Point(4, 22);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(434, 414);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "Serial Monitor";
            // 
            // UserConfigApp
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(493, 478);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.comPorts);
            this.Controls.Add(this.comPortLBL);
            this.Name = "UserConfigApp";
            this.Text = "User Configuration";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.UserConfigApp_FormClosing);
            this.sensorsGRP.ResumeLayout(false);
            this.sensorsGRP.PerformLayout();
            this.networkGRP.ResumeLayout(false);
            this.networkGRP.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            this.tabPage2.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.tabPage3.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button button1;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox comPorts;
        private System.Windows.Forms.Label comPortLBL;
        private System.Windows.Forms.Label psswrdLBL;
        private System.Windows.Forms.Label ssidLBL;
        private System.Windows.Forms.TextBox psswrdTB;
        private System.Windows.Forms.TextBox ssidTB;
        private System.Windows.Forms.GroupBox sensorsGRP;
        private System.Windows.Forms.CheckBox soundCB;
        private System.Windows.Forms.TextBox vib1TB;
        private System.Windows.Forms.TextBox sound1TB;
        private System.Windows.Forms.TextBox levelTB;
        private System.Windows.Forms.TextBox lightTB;
        private System.Windows.Forms.TextBox tempTB;
        private System.Windows.Forms.TextBox motionTB;
        private System.Windows.Forms.TextBox waterTB;
        private System.Windows.Forms.CheckBox motionCB;
        private System.Windows.Forms.CheckBox waterCB;
        private System.Windows.Forms.CheckBox lightCB;
        private System.Windows.Forms.CheckBox tempCB;
        private System.Windows.Forms.CheckBox levelCB;
        private System.Windows.Forms.CheckBox vibrationCB;
        private System.Windows.Forms.GroupBox networkGRP;
        private System.Windows.Forms.TextBox dataRecvTB;
        private System.Windows.Forms.CheckBox LogDataCB;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.TextBox vib3TB;
        private System.Windows.Forms.TextBox sound3TB;
        private System.Windows.Forms.TextBox vib2TB;
        private System.Windows.Forms.TextBox sound2TB;
    }
}

