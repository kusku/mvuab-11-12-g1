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
            this.cbBloomEnable = new System.Windows.Forms.CheckBox();
            this.tConfig.SuspendLayout();
            this.tpVideo.SuspendLayout();
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
            this.tConfig.Size = new System.Drawing.Size(705, 220);
            this.tConfig.TabIndex = 1;
            // 
            // tpVideo
            // 
            this.tpVideo.Controls.Add(this.cbBloomEnable);
            this.tpVideo.Controls.Add(this.cbZBlurEnable);
            this.tpVideo.Controls.Add(this.cbMotionBlurEnable);
            this.tpVideo.Controls.Add(this.cbWindowMode);
            this.tpVideo.Controls.Add(this.cbShadowEnable);
            this.tpVideo.Controls.Add(this.lbShadowQuality);
            this.tpVideo.Controls.Add(this.cbShadowQuality);
            this.tpVideo.Controls.Add(this.lbResolution);
            this.tpVideo.Controls.Add(this.cbResolution);
            this.tpVideo.Location = new System.Drawing.Point(4, 22);
            this.tpVideo.Name = "tpVideo";
            this.tpVideo.Padding = new System.Windows.Forms.Padding(3);
            this.tpVideo.Size = new System.Drawing.Size(697, 194);
            this.tpVideo.TabIndex = 0;
            this.tpVideo.Text = "Video";
            this.tpVideo.UseVisualStyleBackColor = true;
            // 
            // cbZBlurEnable
            // 
            this.cbZBlurEnable.AutoSize = true;
            this.cbZBlurEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbZBlurEnable.Location = new System.Drawing.Point(9, 141);
            this.cbZBlurEnable.Name = "cbZBlurEnable";
            this.cbZBlurEnable.Size = new System.Drawing.Size(93, 17);
            this.cbZBlurEnable.TabIndex = 8;
            this.cbZBlurEnable.Text = "Z Blur Enable:";
            this.cbZBlurEnable.UseVisualStyleBackColor = true;
            // 
            // cbMotionBlurEnable
            // 
            this.cbMotionBlurEnable.AutoSize = true;
            this.cbMotionBlurEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbMotionBlurEnable.Location = new System.Drawing.Point(9, 118);
            this.cbMotionBlurEnable.Name = "cbMotionBlurEnable";
            this.cbMotionBlurEnable.Size = new System.Drawing.Size(118, 17);
            this.cbMotionBlurEnable.TabIndex = 7;
            this.cbMotionBlurEnable.Text = "Motion Blur Enable:";
            this.cbMotionBlurEnable.UseVisualStyleBackColor = true;
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
            this.cbShadowEnable.Location = new System.Drawing.Point(6, 53);
            this.cbShadowEnable.Name = "cbShadowEnable";
            this.cbShadowEnable.Size = new System.Drawing.Size(104, 17);
            this.cbShadowEnable.TabIndex = 5;
            this.cbShadowEnable.Text = "Shadow Enable:";
            this.cbShadowEnable.UseVisualStyleBackColor = true;
            // 
            // lbShadowQuality
            // 
            this.lbShadowQuality.AutoSize = true;
            this.lbShadowQuality.Location = new System.Drawing.Point(6, 79);
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
            this.cbShadowQuality.Location = new System.Drawing.Point(96, 76);
            this.cbShadowQuality.Name = "cbShadowQuality";
            this.cbShadowQuality.Size = new System.Drawing.Size(118, 21);
            this.cbShadowQuality.TabIndex = 2;
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
            this.bApply.Location = new System.Drawing.Point(557, 279);
            this.bApply.Name = "bApply";
            this.bApply.Size = new System.Drawing.Size(75, 23);
            this.bApply.TabIndex = 2;
            this.bApply.Text = "Apply";
            this.bApply.UseVisualStyleBackColor = true;
            this.bApply.Click += new System.EventHandler(this.bApply_Click);
            // 
            // bExit
            // 
            this.bExit.Location = new System.Drawing.Point(638, 279);
            this.bExit.Name = "bExit";
            this.bExit.Size = new System.Drawing.Size(75, 23);
            this.bExit.TabIndex = 3;
            this.bExit.Text = "Exit";
            this.bExit.UseVisualStyleBackColor = true;
            this.bExit.Click += new System.EventHandler(this.bExit_Click);
            // 
            // cbBloomEnable
            // 
            this.cbBloomEnable.AutoSize = true;
            this.cbBloomEnable.CheckAlign = System.Drawing.ContentAlignment.MiddleRight;
            this.cbBloomEnable.Location = new System.Drawing.Point(9, 164);
            this.cbBloomEnable.Name = "cbBloomEnable";
            this.cbBloomEnable.Size = new System.Drawing.Size(94, 17);
            this.cbBloomEnable.TabIndex = 9;
            this.cbBloomEnable.Text = "Bloom Enable:";
            this.cbBloomEnable.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(729, 334);
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
    }
}

