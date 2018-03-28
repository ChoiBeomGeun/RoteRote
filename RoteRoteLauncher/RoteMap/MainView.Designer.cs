namespace RoteMapView
{
    partial class MainView
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.splitContainer1 = new System.Windows.Forms.SplitContainer();
            this.pnlControls = new System.Windows.Forms.Panel();
            this.splitContainer2 = new System.Windows.Forms.SplitContainer();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.savebutton = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.treeControls = new System.Windows.Forms.TreeView();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.designModePanel = new ControlDesignMode.ControlDesignModePanel();
            this.propertyGrid1 = new System.Windows.Forms.PropertyGrid();
            this.chkDesignMode = new System.Windows.Forms.CheckBox();
            this.InstantSavingCheck = new System.Windows.Forms.CheckBox();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).BeginInit();
            this.splitContainer1.Panel1.SuspendLayout();
            this.splitContainer1.Panel2.SuspendLayout();
            this.splitContainer1.SuspendLayout();
            this.pnlControls.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).BeginInit();
            this.splitContainer2.Panel1.SuspendLayout();
            this.splitContainer2.Panel2.SuspendLayout();
            this.splitContainer2.SuspendLayout();
            this.designModePanel.SuspendLayout();
            this.SuspendLayout();
            // 
            // splitContainer1
            // 
            this.splitContainer1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer1.FixedPanel = System.Windows.Forms.FixedPanel.Panel1;
            this.splitContainer1.IsSplitterFixed = true;
            this.splitContainer1.Location = new System.Drawing.Point(0, 0);
            this.splitContainer1.Name = "splitContainer1";
            // 
            // splitContainer1.Panel1
            // 
            this.splitContainer1.Panel1.Controls.Add(this.pnlControls);
            // 
            // splitContainer1.Panel2
            // 
            this.splitContainer1.Panel2.Controls.Add(this.designModePanel);
            this.splitContainer1.Size = new System.Drawing.Size(906, 817);
            this.splitContainer1.SplitterDistance = 227;
            this.splitContainer1.SplitterWidth = 3;
            this.splitContainer1.TabIndex = 0;
            this.splitContainer1.SplitterMoved += new System.Windows.Forms.SplitterEventHandler(this.splitContainer1_SplitterMoved);
            // 
            // pnlControls
            // 
            this.pnlControls.Controls.Add(this.splitContainer2);
            this.pnlControls.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pnlControls.Location = new System.Drawing.Point(0, 0);
            this.pnlControls.Name = "pnlControls";
            this.pnlControls.Size = new System.Drawing.Size(227, 817);
            this.pnlControls.TabIndex = 0;
            // 
            // splitContainer2
            // 
            this.splitContainer2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.splitContainer2.Location = new System.Drawing.Point(0, 0);
            this.splitContainer2.Name = "splitContainer2";
            this.splitContainer2.Orientation = System.Windows.Forms.Orientation.Horizontal;
            // 
            // splitContainer2.Panel1
            // 
            this.splitContainer2.Panel1.Controls.Add(this.InstantSavingCheck);
            this.splitContainer2.Panel1.Controls.Add(this.textBox1);
            this.splitContainer2.Panel1.Controls.Add(this.savebutton);
            this.splitContainer2.Panel1.Controls.Add(this.chkDesignMode);
            // 
            // splitContainer2.Panel2
            // 
            this.splitContainer2.Panel2.Controls.Add(this.textBox2);
            this.splitContainer2.Panel2.Controls.Add(this.treeControls);
            this.splitContainer2.Size = new System.Drawing.Size(227, 817);
            this.splitContainer2.SplitterDistance = 81;
            this.splitContainer2.TabIndex = 1;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(105, 35);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(100, 20);
            this.textBox1.TabIndex = 2;
            this.textBox1.Text = "Levelname";
            this.textBox1.TextChanged += new System.EventHandler(this.textBox1_TextChanged);
            // 
            // savebutton
            // 
            this.savebutton.Location = new System.Drawing.Point(11, 35);
            this.savebutton.Name = "savebutton";
            this.savebutton.Size = new System.Drawing.Size(88, 23);
            this.savebutton.TabIndex = 1;
            this.savebutton.Text = "Save Level";
            this.savebutton.UseVisualStyleBackColor = true;
            this.savebutton.Click += new System.EventHandler(this.button1_Click);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(3, 149);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(224, 664);
            this.textBox2.TabIndex = 2;
            // 
            // treeControls
            // 
            this.treeControls.Location = new System.Drawing.Point(0, 0);
            this.treeControls.Name = "treeControls";
            this.treeControls.Size = new System.Drawing.Size(227, 143);
            this.treeControls.TabIndex = 1;
            this.treeControls.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.treeControls_ItemDrag);
            // 
            // designModePanel
            // 
            this.designModePanel.AllowDrop = true;
            this.designModePanel.BackColor = System.Drawing.SystemColors.Control;
            this.designModePanel.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.designModePanel.Controls.Add(this.propertyGrid1);
            this.designModePanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.designModePanel.IsDesignMode = true;
            this.designModePanel.Location = new System.Drawing.Point(0, 0);
            this.designModePanel.Name = "designModePanel";
            this.designModePanel.Size = new System.Drawing.Size(676, 817);
            this.designModePanel.TabIndex = 0;
            // 
            // propertyGrid1
            // 
            this.propertyGrid1.Dock = System.Windows.Forms.DockStyle.Right;
            this.propertyGrid1.Location = new System.Drawing.Point(532, 0);
            this.propertyGrid1.Name = "propertyGrid1";
            this.propertyGrid1.Size = new System.Drawing.Size(142, 815);
            this.propertyGrid1.TabIndex = 1;
            this.propertyGrid1.PropertyValueChanged += new System.Windows.Forms.PropertyValueChangedEventHandler(this.propertyGrid1_PropertyValueChanged);
            this.propertyGrid1.Click += new System.EventHandler(this.propertyGrid1_Click);
            // 
            // chkDesignMode
            // 
            this.chkDesignMode.AutoSize = true;
            this.chkDesignMode.Checked = true;
            this.chkDesignMode.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkDesignMode.Location = new System.Drawing.Point(11, 12);
            this.chkDesignMode.Name = "chkDesignMode";
            this.chkDesignMode.Size = new System.Drawing.Size(74, 17);
            this.chkDesignMode.TabIndex = 0;
            this.chkDesignMode.Text = "Edit Mode";
            this.chkDesignMode.UseVisualStyleBackColor = true;
            this.chkDesignMode.CheckedChanged += new System.EventHandler(this.chkDesignMode_CheckedChanged_1);
            // 
            // InstantSavingCheck
            // 
            this.InstantSavingCheck.AutoSize = true;
            this.InstantSavingCheck.Location = new System.Drawing.Point(105, 13);
            this.InstantSavingCheck.Name = "InstantSavingCheck";
            this.InstantSavingCheck.Size = new System.Drawing.Size(116, 17);
            this.InstantSavingCheck.TabIndex = 3;
            this.InstantSavingCheck.Text = "Instant Save Mode";
            this.InstantSavingCheck.UseVisualStyleBackColor = true;
            // 
            // MainView
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(906, 817);
            this.Controls.Add(this.splitContainer1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.SizableToolWindow;
            this.Name = "MainView";
            this.Text = "MainView";
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainView_KeyDown);
            this.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.MainView_KeyPress);
            this.splitContainer1.Panel1.ResumeLayout(false);
            this.splitContainer1.Panel2.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer1)).EndInit();
            this.splitContainer1.ResumeLayout(false);
            this.pnlControls.ResumeLayout(false);
            this.splitContainer2.Panel1.ResumeLayout(false);
            this.splitContainer2.Panel1.PerformLayout();
            this.splitContainer2.Panel2.ResumeLayout(false);
            this.splitContainer2.Panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.splitContainer2)).EndInit();
            this.splitContainer2.ResumeLayout(false);
            this.designModePanel.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.SplitContainer splitContainer1;
        private System.Windows.Forms.Panel pnlControls;
        private System.Windows.Forms.TreeView treeControls;
        private System.Windows.Forms.SplitContainer splitContainer2;
        private ControlDesignMode.ControlDesignModePanel designModePanel;
        private System.Windows.Forms.PropertyGrid propertyGrid1;
        private System.Windows.Forms.Button savebutton;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.CheckBox chkDesignMode;
        private System.Windows.Forms.CheckBox InstantSavingCheck;
    }
}

