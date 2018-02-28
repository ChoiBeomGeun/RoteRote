namespace WindowsFormsApp6
{
    partial class ArcheTypeeditor
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
            this.ComponentBox = new System.Windows.Forms.ComboBox();
            this.Name = new System.Windows.Forms.TextBox();
            this.NameLabel = new System.Windows.Forms.Label();
            this.ObjectStyleLabe = new System.Windows.Forms.Label();
            this.ObjectstyleCombo = new System.Windows.Forms.ComboBox();
            this.label1 = new System.Windows.Forms.Label();
            this.CompAdd = new System.Windows.Forms.Button();
            this.CompDelete = new System.Windows.Forms.Button();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.SuspendLayout();
            // 
            // ComponentBox
            // 
            this.ComponentBox.FormattingEnabled = true;
            this.ComponentBox.Location = new System.Drawing.Point(126, 88);
            this.ComponentBox.Name = "ComponentBox";
            this.ComponentBox.Size = new System.Drawing.Size(121, 20);
            this.ComponentBox.TabIndex = 0;
            // 
            // Name
            // 
            this.Name.Location = new System.Drawing.Point(126, 12);
            this.Name.Name = "Name";
            this.Name.Size = new System.Drawing.Size(100, 21);
            this.Name.TabIndex = 1;
            this.Name.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // NameLabel
            // 
            this.NameLabel.AutoSize = true;
            this.NameLabel.Location = new System.Drawing.Point(15, 15);
            this.NameLabel.Name = "NameLabel";
            this.NameLabel.Size = new System.Drawing.Size(101, 12);
            this.NameLabel.TabIndex = 2;
            this.NameLabel.Text = "ArcheTypeName";
            this.NameLabel.Click += new System.EventHandler(this.NameLabel_Click);
            // 
            // ObjectStyleLabe
            // 
            this.ObjectStyleLabe.AutoSize = true;
            this.ObjectStyleLabe.Location = new System.Drawing.Point(15, 52);
            this.ObjectStyleLabe.Name = "ObjectStyleLabe";
            this.ObjectStyleLabe.Size = new System.Drawing.Size(69, 12);
            this.ObjectStyleLabe.TabIndex = 3;
            this.ObjectStyleLabe.Text = "ObjectStyle";
            // 
            // ObjectstyleCombo
            // 
            this.ObjectstyleCombo.FormattingEnabled = true;
            this.ObjectstyleCombo.Location = new System.Drawing.Point(126, 49);
            this.ObjectstyleCombo.Name = "ObjectstyleCombo";
            this.ObjectstyleCombo.Size = new System.Drawing.Size(121, 20);
            this.ObjectstyleCombo.TabIndex = 4;
            this.ObjectstyleCombo.SelectedIndexChanged += new System.EventHandler(this.ObjectstyleCombo_SelectedIndexChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(15, 91);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(77, 12);
            this.label1.TabIndex = 5;
            this.label1.Text = "Components";
            // 
            // CompAdd
            // 
            this.CompAdd.Location = new System.Drawing.Point(253, 88);
            this.CompAdd.Name = "CompAdd";
            this.CompAdd.Size = new System.Drawing.Size(70, 20);
            this.CompAdd.TabIndex = 6;
            this.CompAdd.Text = "Add";
            this.CompAdd.UseVisualStyleBackColor = true;
            // 
            // CompDelete
            // 
            this.CompDelete.Location = new System.Drawing.Point(329, 88);
            this.CompDelete.Name = "CompDelete";
            this.CompDelete.Size = new System.Drawing.Size(75, 20);
            this.CompDelete.TabIndex = 7;
            this.CompDelete.Text = "Delete";
            this.CompDelete.UseVisualStyleBackColor = true;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Location = new System.Drawing.Point(431, 12);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(130, 130);
            this.propertyGrid1.TabIndex = 8;
            // 
            // ArcheTypeeditor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(7F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(672, 570);
            this.Controls.Add(this.propertyGrid1);
            this.Controls.Add(this.CompDelete);
            this.Controls.Add(this.CompAdd);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.ObjectstyleCombo);
            this.Controls.Add(this.ObjectStyleLabe);
            this.Controls.Add(this.NameLabel);
            this.Controls.Add(this.Name);
            this.Controls.Add(this.ComponentBox);
     //       this.Name = "ArcheTypeeditor";
            this.Text = "Archetype Editor";
            this.Load += new System.EventHandler(this.ArcheTypeEditor_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.ComboBox ComponentBox;
        private System.Windows.Forms.TextBox Name;
        private System.Windows.Forms.Label NameLabel;
        private System.Windows.Forms.Label ObjectStyleLabe;
        private System.Windows.Forms.ComboBox ObjectstyleCombo;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Button CompAdd;
        private System.Windows.Forms.Button CompDelete;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
    }
}