namespace RoteRoteLauncherView
{
    partial class ArcheTypeEditorForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ArcheTypeEditorForm));
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.NameLabel = new System.Windows.Forms.Label();
            this.ObjectStyleLabel = new System.Windows.Forms.Label();
            this.ArcheTypeTextBox = new System.Windows.Forms.TextBox();
            this.ObjectStyleBox = new System.Windows.Forms.ComboBox();
            this.ComponentBox = new System.Windows.Forms.ComboBox();
            this.ComponentLabel = new System.Windows.Forms.Label();
            this.Add = new System.Windows.Forms.Button();
            this.DeleteB = new System.Windows.Forms.Button();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.Savebutton1 = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.SuspendLayout();
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Location = new System.Drawing.Point(303, 13);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(172, 333);
            this.propertyGrid1.TabIndex = 0;
            // 
            // NameLabel
            // 
            this.NameLabel.AutoSize = true;
            this.NameLabel.Location = new System.Drawing.Point(11, 24);
            this.NameLabel.Name = "NameLabel";
            this.NameLabel.Size = new System.Drawing.Size(87, 13);
            this.NameLabel.TabIndex = 1;
            this.NameLabel.Text = "ArcheTypeName";
            // 
            // ObjectStyleLabel
            // 
            this.ObjectStyleLabel.AutoSize = true;
            this.ObjectStyleLabel.Location = new System.Drawing.Point(13, 62);
            this.ObjectStyleLabel.Name = "ObjectStyleLabel";
            this.ObjectStyleLabel.Size = new System.Drawing.Size(61, 13);
            this.ObjectStyleLabel.TabIndex = 2;
            this.ObjectStyleLabel.Text = "ObjectStyle";
            // 
            // ArcheTypeTextBox
            // 
            this.ArcheTypeTextBox.Location = new System.Drawing.Point(103, 21);
            this.ArcheTypeTextBox.Name = "ArcheTypeTextBox";
            this.ArcheTypeTextBox.Size = new System.Drawing.Size(86, 20);
            this.ArcheTypeTextBox.TabIndex = 3;
            // 
            // ObjectStyleBox
            // 
            this.ObjectStyleBox.FormattingEnabled = true;
            this.ObjectStyleBox.Location = new System.Drawing.Point(103, 59);
            this.ObjectStyleBox.Name = "ObjectStyleBox";
            this.ObjectStyleBox.Size = new System.Drawing.Size(86, 21);
            this.ObjectStyleBox.TabIndex = 4;
            // 
            // ComponentBox
            // 
            this.ComponentBox.FormattingEnabled = true;
            this.ComponentBox.Location = new System.Drawing.Point(103, 102);
            this.ComponentBox.Name = "ComponentBox";
            this.ComponentBox.Size = new System.Drawing.Size(86, 21);
            this.ComponentBox.TabIndex = 5;
            this.ComponentBox.SelectedIndexChanged += new System.EventHandler(this.ComponentBox_SelectedIndexChanged);
            // 
            // ComponentLabel
            // 
            this.ComponentLabel.AutoSize = true;
            this.ComponentLabel.Location = new System.Drawing.Point(13, 105);
            this.ComponentLabel.Name = "ComponentLabel";
            this.ComponentLabel.Size = new System.Drawing.Size(61, 13);
            this.ComponentLabel.TabIndex = 6;
            this.ComponentLabel.Text = "Component";
            // 
            // Add
            // 
            this.Add.Location = new System.Drawing.Point(196, 102);
            this.Add.Name = "Add";
            this.Add.Size = new System.Drawing.Size(45, 25);
            this.Add.TabIndex = 7;
            this.Add.Text = "Add";
            this.Add.UseVisualStyleBackColor = true;
            this.Add.Click += new System.EventHandler(this.Add_Click);
            // 
            // DeleteB
            // 
            this.DeleteB.Location = new System.Drawing.Point(246, 102);
            this.DeleteB.Name = "DeleteB";
            this.DeleteB.Size = new System.Drawing.Size(45, 25);
            this.DeleteB.TabIndex = 8;
            this.DeleteB.Text = "Delete";
            this.DeleteB.UseVisualStyleBackColor = true;
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(15, 155);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(94, 173);
            this.listBox1.TabIndex = 9;
            this.listBox1.SelectedIndexChanged += new System.EventHandler(this.listBox1_SelectedIndexChanged);
            // 
            // Savebutton1
            // 
            this.Savebutton1.Location = new System.Drawing.Point(23, 348);
            this.Savebutton1.Name = "Savebutton1";
            this.Savebutton1.Size = new System.Drawing.Size(75, 23);
            this.Savebutton1.TabIndex = 10;
            this.Savebutton1.Text = "Save";
            this.Savebutton1.UseVisualStyleBackColor = true;
            this.Savebutton1.Click += new System.EventHandler(this.Savebutton1_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.FileOk += new System.ComponentModel.CancelEventHandler(this.saveFileDialog1_FileOk);
            // 
            // ArcheTypeEditorForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(486, 399);
            this.Controls.Add(this.Savebutton1);
            this.Controls.Add(this.listBox1);
            this.Controls.Add(this.DeleteB);
            this.Controls.Add(this.Add);
            this.Controls.Add(this.ComponentLabel);
            this.Controls.Add(this.ComponentBox);
            this.Controls.Add(this.ObjectStyleBox);
            this.Controls.Add(this.ArcheTypeTextBox);
            this.Controls.Add(this.ObjectStyleLabel);
            this.Controls.Add(this.NameLabel);
            this.Controls.Add(this.propertyGrid1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "ArcheTypeEditorForm";
            this.Text = "ArcheTypeEditor";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.Label NameLabel;
        private System.Windows.Forms.Label ObjectStyleLabel;
        private System.Windows.Forms.TextBox ArcheTypeTextBox;
        private System.Windows.Forms.ComboBox ObjectStyleBox;
        private System.Windows.Forms.ComboBox ComponentBox;
        private System.Windows.Forms.Label ComponentLabel;
        private System.Windows.Forms.Button Add;
        private System.Windows.Forms.Button DeleteB;
        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button Savebutton1;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
    }
}