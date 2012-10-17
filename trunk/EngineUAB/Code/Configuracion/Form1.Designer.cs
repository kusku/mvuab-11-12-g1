namespace Configuracion
{
    partial class Form1
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.lbTitle = new System.Windows.Forms.Label();
            this.tConfig = new System.Windows.Forms.TabControl();
            this.tpVideo = new System.Windows.Forms.TabPage();
            this.lbParticleQuality = new System.Windows.Forms.Label();
            this.cbParticleQuality = new System.Windows.Forms.ComboBox();
            this.cbBloomEnable = new System.Windows.Forms.CheckBox();
            this.cbZBlurEnable = new System.Windows.Forms.CheckBox();
            this.cbMotionBlurEnable = new System.Windows.Forms.CheckBox();
            this.cbWindowMode = new System.Windows.Forms.CheckBox();
            this.cbShadowEnable = new System.Windows.Forms.CheckBox();
            this.lbShadowQuality = new System.Windows.Forms.Label();
            this.cbShadowQuality = new System.Windows.Forms.ComboBox();
            this.lbResolution = new System.Windows.Forms.Label();
            this.cbResolution = new System.Windows.Forms.ComboBox();
            this.bApply = new System.Windows.Forms.Button();
            this.bExit = new System.Windows.Forms.Button();
            this.gbSettings = new System.Windows.Forms.GroupBox();
            this.lbVideoSettings = new System.Windows.Forms.Label();
            this.cbSettings = new System.Windows.Forms.ComboBox();
            this.tConfig.SuspendLayout();
            this.tpVideo.SuspendLayout();
            this.gbSettings.SuspendLayout();
            this.SuspendLayout();
            // 
            // lbTitle
            // 
            this.lbTitle.AutoSize = true;
            this.lbTitle.Font = new System.Drawing.Font("Microsoft Sans Serif", 24F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lbTitle.ForeColor = System.Drawing.Color.DarkRed;
            this.lbTitle.Location = new System.Drawing.Point(5, 9);
            this.lbTitle.Name = "lbTitle";
            this.lbTitle.Size = new System.Drawing.Size(375, 37);
            this.lbTitle.TabIndex = 0;
            this.lbTitle.Text = "Red Forest Configuration";
            // 
            // tConfig
            // 
            this.tConfig.Controls.Add(this.tpVideo);
            this.tConfig.Location = new System.Drawing.Point(12, 53);
            this.tConfig.Name = "tConfig";
            this.tConfig.SelectedIndex = 0;
            this.tConfig.Size = new System.Drawing.Size(705, 295);
            this.tConfig.TabIndex = 1;
            // 
            // tpVideo
            // 
            this.tpVideo.Controls.Add(this.lbVideoSettings);
            this.tpVideo.Controls.Add(this.gbSettings);
            this.tpVideo.Controls.Add(this.cbSettings);
            this.tpVideo.Controls.Add(this.cbWindowMode);
            this.tpVideo.Controls.Add(this.lbResolution);
            this.tpVideo.Controls.Add(this.cbResolution);
            this.tpVideo.Location = new System.Drawing.Point(4, 22);
            this.tpVideo.Name = "tpVideo";
            this.tpVideo.Padding = new System.Windows.Forms.Padding(3);
            this.tpVideo.Size = new System.Drawing.Size(697, 269);
            this.tpVideo.TabIndex = 0;
            this.tpVideo.Text = "Video";
            this.tpVideo.UseVisualStyleBackColor = true;
            // 
            // lbParticleQuality
            // 
            this.lbParticleQuality.AutoSize = true;
            this.lbParticleQuality.Location = new System.Drawing.Point(12, 140);
            this.lbParticleQuality.Name = "lbParticleQuality";
            this.lbParticleQuality.Size = new System.Drawing.Size(80, 13);
            this.lbParticleQuality.TabIndex = 11;
            this.lbParticleQuality.Text = "Particle Quality:";
            // 
            // cbParticleQuality
            // 
            this.cbParticleQuality.FormattingEnabled = true;
            this.cbParticleQuality.Items.AddRange(new object[] {
            "Low",
            "Medium",
            "High"});
            this.cbParticleQuality.Location = new System.Drawing.Point(102, 137);
            this.cbParticleQuality.Name = "cbParticleQuality";
            this.cbParticleQuality.Size = new System.Drawing.Size(118, 21);
            this.cbParticleQuality.TabIndex = 10;
            this.cbParticleQuality.SelectedIndexChanged += new System.EventHandler(this.cbParticleQuality_SelectedIndexChanged);
            // 
            // cbBloomEnable
            // 
            this.cbBloomEnable.AutoSize = true;
            this.cbBloomEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbBloomEnable.Location = new System.Drawing.Point(10, 102);
            this.cbBloomEnable.Name = "cbBloomEnable";
            this.cbBloomEnable.Size = new System.Drawing.Size(94, 17);
            this.cbBloomEnable.TabIndex = 9;
            this.cbBloomEnable.Text = "Bloom Enable:";
            this.cbBloomEnable.UseVisualStyleBackColor = true;
            this.cbBloomEnable.CheckedChanged += new System.EventHandler(this.cbBloomEnable_CheckedChanged);
            // 
            // cbZBlurEnable
            // 
            this.cbZBlurEnable.AutoSize = true;
            this.cbZBlurEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbZBlurEnable.Location = new System.Drawing.Point(10, 79);
            this.cbZBlurEnable.Name = "cbZBlurEnable";
            this.cbZBlurEnable.Size = new System.Drawing.Size(93, 17);
            this.cbZBlurEnable.TabIndex = 8;
            this.cbZBlurEnable.Text = "Z Blur Enable:";
            this.cbZBlurEnable.UseVisualStyleBackColor = true;
            this.cbZBlurEnable.CheckedChanged += new System.EventHandler(this.cbZBlurEnable_CheckedChanged);
            // 
            // cbMotionBlurEnable
            // 
            this.cbMotionBlurEnable.AutoSize = true;
            this.cbMotionBlurEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbMotionBlurEnable.Location = new System.Drawing.Point(10, 56);
            this.cbMotionBlurEnable.Name = "cbMotionBlurEnable";
            this.cbMotionBlurEnable.Size = new System.Drawing.Size(118, 17);
            this.cbMotionBlurEnable.TabIndex = 7;
            this.cbMotionBlurEnable.Text = "Motion Blur Enable:";
            this.cbMotionBlurEnable.UseVisualStyleBackColor = true;
            this.cbMotionBlurEnable.CheckedChanged += new System.EventHandler(this.cbMotionBlurEnable_CheckedChanged);
            // 
            // cbWindowMode
            // 
            this.cbWindowMode.AutoSize = true;
            this.cbWindowMode.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbWindowMode.Location = new System.Drawing.Point(240, 14);
            this.cbWindowMode.Name = "cbWindowMode";
            this.cbWindowMode.Size = new System.Drawing.Size(134, 17);
            this.cbWindowMode.TabIndex = 6;
            this.cbWindowMode.Text = "Window Mode Enable:";
            this.cbWindowMode.UseVisualStyleBackColor = true;
            // 
            // cbShadowEnable
            // 
            this.cbShadowEnable.AutoSize = true;
            this.cbShadowEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbShadowEnable.Location = new System.Drawing.Point(10, 19);
            this.cbShadowEnable.Name = "cbShadowEnable";
            this.cbShadowEnable.Size = new System.Drawing.Size(104, 17);
            this.cbShadowEnable.TabIndex = 5;
            this.cbShadowEnable.Text = "Shadow Enable:";
            this.cbShadowEnable.UseVisualStyleBackColor = true;
            this.cbShadowEnable.CheckedChanged += new System.EventHandler(this.cbShadowEnable_CheckedChanged);
            // 
            // lbShadowQuality
            // 
            this.lbShadowQuality.AutoSize = true;
            this.lbShadowQuality.Location = new System.Drawing.Point(136, 20);
            this.lbShadowQuality.Name = "lbShadowQuality";
            this.lbShadowQuality.Size = new System.Drawing.Size(84, 13);
            this.lbShadowQuality.TabIndex = 3;
            this.lbShadowQuality.Text = "Shadow Quality:";
            // 
            // cbShadowQuality
            // 
            this.cbShadowQuality.FormattingEnabled = true;
            this.cbShadowQuality.Items.AddRange(new object[] {
            "Low",
            "Medium",
            "High",
            "Very High"});
            this.cbShadowQuality.Location = new System.Drawing.Point(226, 17);
            this.cbShadowQuality.Name = "cbShadowQuality";
            this.cbShadowQuality.Size = new System.Drawing.Size(118, 21);
            this.cbShadowQuality.TabIndex = 2;
            this.cbShadowQuality.SelectedIndexChanged += new System.EventHandler(this.cbShadowQuality_SelectedIndexChanged);
            // 
            // lbResolution
            // 
            this.lbResolution.AutoSize = true;
            this.lbResolution.Location = new System.Drawing.Point(6, 15);
            this.lbResolution.Name = "lbResolution";
            this.lbResolution.Size = new System.Drawing.Size(60, 13);
            this.lbResolution.TabIndex = 1;
            this.lbResolution.Text = "Resolution:";
            // 
            // cbResolution
            // 
            this.cbResolution.FormattingEnabled = true;
            this.cbResolution.Items.AddRange(new object[] {
            "640x360",
            "800x600",
            "1024x768",
            "1280x720",
            "1280x1024",
            "1920x1080"});
            this.cbResolution.Location = new System.Drawing.Point(72, 12);
            this.cbResolution.Name = "cbResolution";
            this.cbResolution.Size = new System.Drawing.Size(162, 21);
            this.cbResolution.TabIndex = 0;
            // 
            // bApply
            // 
            this.bApply.Location = new System.Drawing.Point(557, 354);
            this.bApply.Name = "bApply";
            this.bApply.Size = new System.Drawing.Size(75, 23);
            this.bApply.TabIndex = 2;
            this.bApply.Text = "Apply";
            this.bApply.UseVisualStyleBackColor = true;
            this.bApply.Click += new System.EventHandler(this.bApply_Click);
            // 
            // bExit
            // 
            this.bExit.Location = new System.Drawing.Point(638, 354);
            this.bExit.Name = "bExit";
            this.bExit.Size = new System.Drawing.Size(75, 23);
            this.bExit.TabIndex = 3;
            this.bExit.Text = "Exit";
            this.bExit.UseVisualStyleBackColor = true;
            this.bExit.Click += new System.EventHandler(this.bExit_Click);
            // 
            // gbSettings
            // 
            this.gbSettings.Controls.Add(this.cbShadowEnable);
            this.gbSettings.Controls.Add(this.lbParticleQuality);
            this.gbSettings.Controls.Add(this.cbShadowQuality);
            this.gbSettings.Controls.Add(this.cbParticleQuality);
            this.gbSettings.Controls.Add(this.lbShadowQuality);
            this.gbSettings.Controls.Add(this.cbBloomEnable);
            this.gbSettings.Controls.Add(this.cbMotionBlurEnable);
            this.gbSettings.Controls.Add(this.cbZBlurEnable);
            this.gbSettings.Location = new System.Drawing.Point(9, 91);
            this.gbSettings.Name = "gbSettings";
            this.gbSettings.Size = new System.Drawing.Size(402, 172);
            this.gbSettings.TabIndex = 12;
            this.gbSettings.TabStop = false;
            this.gbSettings.Text = "Settings";
            // 
            // lbVideoSettings
            // 
            this.lbVideoSettings.AutoSize = true;
            this.lbVideoSettings.Location = new System.Drawing.Point(6, 61);
            this.lbVideoSettings.Name = "lbVideoSettings";
            this.lbVideoSettings.Size = new System.Drawing.Size(78, 13);
            this.lbVideoSettings.TabIndex = 13;
            this.lbVideoSettings.Text = "Video Settings:";
            // 
            // cbSettings
            // 
            this.cbSettings.FormattingEnabled = true;
            this.cbSettings.Items.AddRange(new object[] {
            "Low",
            "Medium",
            "High",
            "Very High",
            "Custom"});
            this.cbSettings.Location = new System.Drawing.Point(96, 58);
            this.cbSettings.Name = "cbSettings";
            this.cbSettings.Size = new System.Drawing.Size(118, 21);
            this.cbSettings.TabIndex = 12;
            this.cbSettings.SelectedIndexChanged += new System.EventHandler(this.cbSettings_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(729, 415);
            this.Controls.Add(this.bExit);
            this.Controls.Add(this.bApply);
            this.Controls.Add(this.tConfig);
            this.Controls.Add(this.lbTitle);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "Red Forest Configuration";
            this.tConfig.ResumeLayout(false);
            this.tpVideo.ResumeLayout(false);
            this.tpVideo.PerformLayout();
            this.gbSettings.ResumeLayout(false);
            this.gbSettings.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lbTitle;
        private System.Windows.Forms.TabControl tConfig;
        private System.Windows.Forms.TabPage tpVideo;
        private System.Windows.Forms.Button bApply;
        private System.Windows.Forms.Button bExit;
        private System.Windows.Forms.Label lbResolution;
        private System.Windows.Forms.ComboBox cbResolution;
        private System.Windows.Forms.CheckBox cbMotionBlurEnable;
        private System.Windows.Forms.CheckBox cbWindowMode;
        private System.Windows.Forms.CheckBox cbShadowEnable;
        private System.Windows.Forms.Label lbShadowQuality;
        private System.Windows.Forms.ComboBox cbShadowQuality;
        private System.Windows.Forms.CheckBox cbZBlurEnable;
        private System.Windows.Forms.CheckBox cbBloomEnable;
        private System.Windows.Forms.Label lbParticleQuality;
        private System.Windows.Forms.ComboBox cbParticleQuality;
        private System.Windows.Forms.Label lbVideoSettings;
        private System.Windows.Forms.GroupBox gbSettings;
        private System.Windows.Forms.ComboBox cbSettings;
    }
}

