// Game Character Manager using Prototype Pattern - Project Structure

// 1. Character.cs - Base character class implementing ICloneable
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.Json.Serialization;

namespace GameCharacterManager
{
    [Serializable]
    public class Character : ICloneable
    {
        // Basic characteristics
        public string Name { get; set; }
        public int Level { get; set; }
        public int Health { get; set; }
        public int Mana { get; set; }
        public List<string> Abilities { get; set; }

        // Specific properties
        public string WeaponType { get; set; }
        public CharacterClass Class { get; set; }
        public string ArmorType { get; set; }

        // Default constructor
        public Character()
        {
            Name = "New Character";
            Level = 1;
            Health = 100;
            Mana = 100;
            Abilities = new List<string>();
            WeaponType = "None";
            Class = CharacterClass.Warrior;
            ArmorType = "Light";
        }

        // Constructor with parameters
        public Character(string name, int level, int health, int mana, List<string> abilities, 
                        string weaponType, CharacterClass characterClass, string armorType)
        {
            Name = name;
            Level = level;
            Health = health;
            Mana = mana;
            Abilities = abilities ?? new List<string>();
            WeaponType = weaponType;
            Class = characterClass;
            ArmorType = armorType;
        }

        // Clone method from ICloneable interface
        public object Clone()
        {
            // Deep copy of the abilities list
            List<string> abilitiesCopy = new List<string>(Abilities);
            
            return new Character(
                Name + " (Copy)",
                Level,
                Health,
                Mana,
                abilitiesCopy,
                WeaponType,
                Class,
                ArmorType
            );
        }

        // Override ToString for display in UI
        public override string ToString()
        {
            return $"{Name} - Level {Level} {Class}";
        }
    }

    // Character class enum
    public enum CharacterClass
    {
        Warrior,
        Mage,
        Rogue,
        Priest,
        Hunter
    }
}

// 2. CharacterRepository.cs - For saving and loading characters
using System;
using System.Collections.Generic;
using System.IO;
using System.Text.Json;
using System.Xml.Serialization;

namespace GameCharacterManager
{
    public class CharacterRepository
    {
        private const string JsonFilePath = "characters.json";
        private const string XmlFilePath = "characters.xml";

        // Save characters to JSON file
        public void SaveToJson(List<Character> characters)
        {
            var options = new JsonSerializerOptions { WriteIndented = true };
            string jsonString = JsonSerializer.Serialize(characters, options);
            File.WriteAllText(JsonFilePath, jsonString);
        }

        // Load characters from JSON file
        public List<Character> LoadFromJson()
        {
            if (!File.Exists(JsonFilePath))
                return new List<Character>();

            string jsonString = File.ReadAllText(JsonFilePath);
            return JsonSerializer.Deserialize<List<Character>>(jsonString) ?? new List<Character>();
        }

        // Save characters to XML file
        public void SaveToXml(List<Character> characters)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(List<Character>));
            using (FileStream fs = new FileStream(XmlFilePath, FileMode.Create))
            {
                serializer.Serialize(fs, characters);
            }
        }

        // Load characters from XML file
        public List<Character> LoadFromXml()
        {
            if (!File.Exists(XmlFilePath))
                return new List<Character>();

            XmlSerializer serializer = new XmlSerializer(typeof(List<Character>));
            using (FileStream fs = new FileStream(XmlFilePath, FileMode.Open))
            {
                return (List<Character>)serializer.Deserialize(fs) ?? new List<Character>();
            }
        }
    }
}

// 3. MainForm.cs - Main application window
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace GameCharacterManager
{
    public partial class MainForm : Form
    {
        private List<Character> _characters;
        private CharacterRepository _repository;

        public MainForm()
        {
            InitializeComponent();
            _repository = new CharacterRepository();
            _characters = new List<Character>();
            
            // Load characters on startup
            try
            {
                _characters = _repository.LoadFromJson();
                UpdateCharactersList();
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading characters: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void UpdateCharactersList()
        {
            listBoxCharacters.Items.Clear();
            foreach (var character in _characters)
            {
                listBoxCharacters.Items.Add(character);
            }
        }

        private void btnCreate_Click(object sender, EventArgs e)
        {
            using (CharacterForm form = new CharacterForm())
            {
                if (form.ShowDialog() == DialogResult.OK)
                {
                    _characters.Add(form.Character);
                    UpdateCharactersList();
                }
            }
        }

        private void btnClone_Click(object sender, EventArgs e)
        {
            if (listBoxCharacters.SelectedItem is Character selectedCharacter)
            {
                Character clonedCharacter = (Character)selectedCharacter.Clone();
                _characters.Add(clonedCharacter);
                UpdateCharactersList();
                listBoxCharacters.SelectedItem = clonedCharacter;
            }
            else
            {
                MessageBox.Show("Please select a character to clone.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void btnEdit_Click(object sender, EventArgs e)
        {
            if (listBoxCharacters.SelectedItem is Character selectedCharacter)
            {
                int index = _characters.IndexOf(selectedCharacter);
                using (CharacterForm form = new CharacterForm(selectedCharacter))
                {
                    if (form.ShowDialog() == DialogResult.OK)
                    {
                        _characters[index] = form.Character;
                        UpdateCharactersList();
                    }
                }
            }
            else
            {
                MessageBox.Show("Please select a character to edit.", "Information", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
        }

        private void btnSaveJson_Click(object sender, EventArgs e)
        {
            try
            {
                _repository.SaveToJson(_characters);
                MessageBox.Show("Characters saved to JSON successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error saving to JSON: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnSaveXml_Click(object sender, EventArgs e)
        {
            try
            {
                _repository.SaveToXml(_characters);
                MessageBox.Show("Characters saved to XML successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error saving to XML: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnLoadJson_Click(object sender, EventArgs e)
        {
            try
            {
                _characters = _repository.LoadFromJson();
                UpdateCharactersList();
                MessageBox.Show("Characters loaded from JSON successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading from JSON: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void btnLoadXml_Click(object sender, EventArgs e)
        {
            try
            {
                _characters = _repository.LoadFromXml();
                UpdateCharactersList();
                MessageBox.Show("Characters loaded from XML successfully!", "Success", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Error loading from XML: {ex.Message}", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}

// 4. CharacterForm.cs - Form for creating/editing characters
using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace GameCharacterManager
{
    public partial class CharacterForm : Form
    {
        public Character Character { get; private set; }

        // Constructor for creating a new character
        public CharacterForm()
        {
            InitializeComponent();
            Character = new Character();
            SetupForm();
        }

        // Constructor for editing an existing character
        public CharacterForm(Character character)
        {
            InitializeComponent();
            Character = (Character)character.Clone();
            SetupForm();
            
            // Populate form with character data
            txtName.Text = Character.Name;
            numLevel.Value = Character.Level;
            numHealth.Value = Character.Health;
            numMana.Value = Character.Mana;
            cmbClass.SelectedItem = Character.Class;
            txtWeapon.Text = Character.WeaponType;
            txtArmor.Text = Character.ArmorType;
            
            // Populate abilities list
            listBoxAbilities.Items.Clear();
            foreach (var ability in Character.Abilities)
            {
                listBoxAbilities.Items.Add(ability);
            }
        }

        private void SetupForm()
        {
            // Set up the class combo box
            cmbClass.DataSource = Enum.GetValues(typeof(CharacterClass));
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            // Update character properties from form inputs
            Character.Name = txtName.Text;
            Character.Level = (int)numLevel.Value;
            Character.Health = (int)numHealth.Value;
            Character.Mana = (int)numMana.Value;
            Character.Class = (CharacterClass)cmbClass.SelectedItem;
            Character.WeaponType = txtWeapon.Text;
            Character.ArmorType = txtArmor.Text;
            
            // Update abilities
            Character.Abilities.Clear();
            foreach (var item in listBoxAbilities.Items)
            {
                Character.Abilities.Add(item.ToString());
            }
            
            DialogResult = DialogResult.OK;
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
        }

        private void btnAddAbility_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(txtAbility.Text))
            {
                listBoxAbilities.Items.Add(txtAbility.Text);
                txtAbility.Clear();
                txtAbility.Focus();
            }
        }

        private void btnRemoveAbility_Click(object sender, EventArgs e)
        {
            if (listBoxAbilities.SelectedIndex != -1)
            {
                listBoxAbilities.Items.RemoveAt(listBoxAbilities.SelectedIndex);
            }
        }
    }
}

// 5. Designer files for MainForm.Designer.cs
namespace GameCharacterManager
{
    partial class MainForm
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.listBoxCharacters = new System.Windows.Forms.ListBox();
            this.btnCreate = new System.Windows.Forms.Button();
            this.btnClone = new System.Windows.Forms.Button();
            this.btnEdit = new System.Windows.Forms.Button();
            this.btnSaveJson = new System.Windows.Forms.Button();
            this.btnSaveXml = new System.Windows.Forms.Button();
            this.btnLoadJson = new System.Windows.Forms.Button();
            this.btnLoadXml = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.SuspendLayout();
            // 
            // listBoxCharacters
            // 
            this.listBoxCharacters.FormattingEnabled = true;
            this.listBoxCharacters.ItemHeight = 16;
            this.listBoxCharacters.Location = new System.Drawing.Point(16, 36);
            this.listBoxCharacters.Name = "listBoxCharacters";
            this.listBoxCharacters.Size = new System.Drawing.Size(380, 324);
            this.listBoxCharacters.TabIndex = 0;
            // 
            // btnCreate
            // 
            this.btnCreate.Location = new System.Drawing.Point(413, 36);
            this.btnCreate.Name = "btnCreate";
            this.btnCreate.Size = new System.Drawing.Size(160, 35);
            this.btnCreate.TabIndex = 1;
            this.btnCreate.Text = "Create New";
            this.btnCreate.UseVisualStyleBackColor = true;
            this.btnCreate.Click += new System.EventHandler(this.btnCreate_Click);
            // 
            // btnClone
            // 
            this.btnClone.Location = new System.Drawing.Point(413, 85);
            this.btnClone.Name = "btnClone";
            this.btnClone.Size = new System.Drawing.Size(160, 35);
            this.btnClone.TabIndex = 2;
            this.btnClone.Text = "Clone Selected";
            this.btnClone.UseVisualStyleBackColor = true;
            this.btnClone.Click += new System.EventHandler(this.btnClone_Click);
            // 
            // btnEdit
            // 
            this.btnEdit.Location = new System.Drawing.Point(413, 134);
            this.btnEdit.Name = "btnEdit";
            this.btnEdit.Size = new System.Drawing.Size(160, 35);
            this.btnEdit.TabIndex = 3;
            this.btnEdit.Text = "Edit Selected";
            this.btnEdit.UseVisualStyleBackColor = true;
            this.btnEdit.Click += new System.EventHandler(this.btnEdit_Click);
            // 
            // btnSaveJson
            // 
            this.btnSaveJson.Location = new System.Drawing.Point(413, 223);
            this.btnSaveJson.Name = "btnSaveJson";
            this.btnSaveJson.Size = new System.Drawing.Size(160, 35);
            this.btnSaveJson.TabIndex = 4;
            this.btnSaveJson.Text = "Save as JSON";
            this.btnSaveJson.UseVisualStyleBackColor = true;
            this.btnSaveJson.Click += new System.EventHandler(this.btnSaveJson_Click);
            // 
            // btnSaveXml
            // 
            this.btnSaveXml.Location = new System.Drawing.Point(413, 264);
            this.btnSaveXml.Name = "btnSaveXml";
            this.btnSaveXml.Size = new System.Drawing.Size(160, 35);
            this.btnSaveXml.TabIndex = 5;
            this.btnSaveXml.Text = "Save as XML";
            this.btnSaveXml.UseVisualStyleBackColor = true;
            this.btnSaveXml.Click += new System.EventHandler(this.btnSaveXml_Click);
            // 
            // btnLoadJson
            // 
            this.btnLoadJson.Location = new System.Drawing.Point(413, 305);
            this.btnLoadJson.Name = "btnLoadJson";
            this.btnLoadJson.Size = new System.Drawing.Size(160, 35);
            this.btnLoadJson.TabIndex = 6;
            this.btnLoadJson.Text = "Load from JSON";
            this.btnLoadJson.UseVisualStyleBackColor = true;
            this.btnLoadJson.Click += new System.EventHandler(this.btnLoadJson_Click);
            // 
            // btnLoadXml
            // 
            this.btnLoadXml.Location = new System.Drawing.Point(413, 346);
            this.btnLoadXml.Name = "btnLoadXml";
            this.btnLoadXml.Size = new System.Drawing.Size(160, 35);
            this.btnLoadXml.TabIndex = 7;
            this.btnLoadXml.Text = "Load from XML";
            this.btnLoadXml.UseVisualStyleBackColor = true;
            this.btnLoadXml.Click += new System.EventHandler(this.btnLoadXml_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(13, 13);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(140, 18);
            this.label1.TabIndex = 8;
            this.label1.Text = "Game Characters";
            // 
            // groupBox1
            // 
            this.groupBox1.Location = new System.Drawing.Point(403, 11);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(179, 173);
            this.groupBox1.TabIndex = 9;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Character Actions";
            // 
            // groupBox2
            // 
            this.groupBox2.Location = new System.Drawing.Point(403, 202);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(179, 193);
            this.groupBox2.TabIndex = 10;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Save/Load";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(594, 407);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.btnLoadXml);
            this.Controls.Add(this.btnLoadJson);
            this.Controls.Add(this.btnSaveXml);
            this.Controls.Add(this.btnSaveJson);
            this.Controls.Add(this.btnEdit);
            this.Controls.Add(this.btnClone);
            this.Controls.Add(this.btnCreate);
            this.Controls.Add(this.listBoxCharacters);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.groupBox2);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.MaximizeBox = false;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Game Character Manager";
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.ListBox listBoxCharacters;
        private System.Windows.Forms.Button btnCreate;
        private System.Windows.Forms.Button btnClone;
        private System.Windows.Forms.Button btnEdit;
        private System.Windows.Forms.Button btnSaveJson;
        private System.Windows.Forms.Button btnSaveXml;
        private System.Windows.Forms.Button btnLoadJson;
        private System.Windows.Forms.Button btnLoadXml;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.GroupBox groupBox2;
    }
}

// 6. Designer files for CharacterForm.Designer.cs
namespace GameCharacterManager
{
    partial class CharacterForm
    {
        private System.ComponentModel.IContainer components = null;

        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        private void InitializeComponent()
        {
            this.label1 = new System.Windows.Forms.Label();
            this.txtName = new System.Windows.Forms.TextBox();
            this.label2 = new System.Windows.Forms.Label();
            this.numLevel = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.numHealth = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.numMana = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.cmbClass = new System.Windows.Forms.ComboBox();
            this.label6 = new System.Windows.Forms.Label();
            this.txtWeapon = new System.Windows.Forms.TextBox();
            this.label7 = new System.Windows.Forms.Label();
            this.txtArmor = new System.Windows.Forms.TextBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.btnRemoveAbility = new System.Windows.Forms.Button();
            this.btnAddAbility = new System.Windows.Forms.Button();
            this.txtAbility = new System.Windows.Forms.TextBox();
            this.listBoxAbilities = new System.Windows.Forms.ListBox();
            this.btnSave = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.numLevel)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHealth)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMana)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(21, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(44, 16);
            this.label1.TabIndex = 0;
            this.label1.Text = "Name";
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(148, 21);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(257, 22);
            this.txtName.TabIndex = 1;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(21, 61);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(40, 16);
            this.label2.TabIndex = 2;
            this.label2.Text = "Level";
            // 
            // numLevel
            // 
            this.numLevel.Location = new System.Drawing.Point(148, 59);
            this.numLevel.Maximum = new decimal(new int[] {
            100,
            0,
            0,
            0});
            this.numLevel.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numLevel.Name = "numLevel";
            this.numLevel.Size = new System.Drawing.Size(120, 22);
            this.numLevel.TabIndex = 3;
            this.numLevel.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(21, 99);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(48, 16);
            this.label3.TabIndex = 4;
            this.label3.Text = "Health";
            // 
            // numHealth
            // 
            this.numHealth.Location = new System.Drawing.Point(148, 97);
            this.numHealth.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numHealth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numHealth.Name = "numHealth";
            this.numHealth.Size = new System.Drawing.Size(120, 22);
            this.numHealth.TabIndex = 5;
            this.numHealth.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(21, 139);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(42, 16);
            this.label4.TabIndex = 6;
            this.label4.Text = "Mana";
            // 
            // numMana
            // 
            this.numMana.Location = new System.Drawing.Point(148, 137);
            this.numMana.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numMana.Name = "numMana";
            this.numMana.Size = new System.Drawing.Size(120, 22);
            this.numMana.TabIndex = 7;
            this.numMana.Value = new decimal(new int[] {
            100,
            0,
            0,
            0});
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(21, 177);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(42, 16);
            this.label5.TabIndex = 8;
            this.label5.Text = "Class";
            // 
            // cmbClass
            // 
            this.cmbClass.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbClass.FormattingEnabled = true;
            this.cmbClass.Location = new System.Drawing.Point(148, 174);
            this.cmbClass.Name = "cmbClass";
            this.cmbClass.Size = new System.Drawing.Size(257, 24);
            this.cmbClass.TabIndex = 9;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(21, 217);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(97, 16);
            this.label6.TabIndex = 10;
            this.label6.Text = "Weapon Type";
            // 
            // txtWeapon
            // 
            this.txtWeapon.Location = new System.Drawing.Point(148, 214);
            this.txtWeapon.Name = "txtWeapon";
            this.txtWeapon.Size = new System.Drawing.Size(257, 22);
            this.txtWeapon.TabIndex = 11;
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(21, 255);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(81, 16);
            this.label7.TabIndex = 12;
            this.label7.Text = "Armor Type";
            // 
            // txtArmor
            // 
            this.txtArmor.Location = new System.Drawing.Point(148, 252);
            this.txtArmor.Name = "txtArmor";
            this.txtArmor.Size = new System.Drawing.Size(257, 22);
            this.txtArmor.TabIndex = 13;
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.btnRemoveAbility);
            this.groupBox1.Controls.Add(this.btnAddAbility);
            this.groupBox1.Controls.Add(this.txtAbility);
            this.groupBox1.Controls.Add(this.listBoxAbilities);
            this.groupBox1.Location = new System.Drawing.Point(24, 291);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(381, 214);
            this.groupBox1.TabIndex = 14;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Abilities";
            // 
            // btnRemoveAbility
            // 
            this.btnRemoveAbility.Location = new System.Drawing.Point(257, 178);
            this.btnRemoveAbility.Name = "btnRemoveAbility";
            this.btnRemoveAbility.Size = new System.Drawing.Size(109, 30);
            this.btnRemoveAbility.TabIndex = 3;
            this.btnRemoveAbility.Text = "Remove";
            this.btnRemoveAbility.UseVisualStyleBackColor = true;
            this.btnRemoveAbility.Click += new System.EventHandler(this.btnRemoveAbility_Click);
            // 
            // btnAddAbility
            // 
            this.btnAddAbility.Location = new System.Drawing.Point(257, 55);
            this.btnAddAbility.Name = "btnAddAbility";
            this.btnAddAbility.Size = new System.Drawing.Size(109, 30);
            this.btnAddAbility.TabIndex = 2;
            this.btnAddAbility.Text = "Add";
            // 
            // txtAbility
            // 
            this.txtAbility.Location = new System.Drawing.Point(15, 27);
            this.txtAbility.Name = "txtAbility";
            this.txtAbility.Size = new System.Drawing.Size(351, 22);
            this.txtAbility.TabIndex = 1;
            // 
            // listBoxAbilities
            // 
            this.listBoxAbilities.FormattingEnabled = true;
            this.listBoxAbilities.ItemHeight = 16;
            this.listBoxAbilities.Location = new System.Drawing.Point(15, 55);
            this.listBoxAbilities.Name = "listBoxAbilities";
            this.listBoxAbilities.Size = new System.Drawing.Size(236, 116);
            this.listBoxAbilities.TabIndex = 0;
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(124, 511);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(109, 35);
            this.btnSave.TabIndex = 15;
            this.btnSave.Text = "Save";
            this.btnSave.UseVisualStyleBackColor = true;
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(239, 511);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(109, 35);
            this.btnCancel.TabIndex = 16;
            this.btnCancel.Text = "Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            this.btnCancel.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // CharacterForm
            // 
            this.AcceptButton = this.btnSave;
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(427, 558);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.txtArmor);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.txtWeapon);
            this.Controls.Add(this.label6);
            this.Controls.Add(this.cmbClass);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.numMana);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.numHealth);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numLevel);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.label1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "CharacterForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Character Editor";
            ((System.ComponentModel.ISupportInitialize)(this.numLevel)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numHealth)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numMana)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();
        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numLevel;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numHealth;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown numMana;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox cmbClass;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.TextBox txtWeapon;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.TextBox txtArmor;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnRemoveAbility;
        private System.Windows.Forms.Button btnAddAbility;
        private System.Windows.Forms.TextBox txtAbility;
        private System.Windows.Forms.ListBox listBoxAbilities;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Button btnCancel;
    }
}

// 7. Program.cs - Application entry point
using System;
using System.Windows.Forms;

namespace GameCharacterManager
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }
}
