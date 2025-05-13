using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text.Json;
using System.Windows.Forms;
using System.Xml.Serialization;

namespace CharacterManagementSystem
{
    public class Program
    {
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new MainForm());
        }
    }

    // Базовий клас персонажа з реалізацією ICloneable
    [Serializable]
    public class Character : ICloneable
    {
        public string Name { get; set; }
        public int Level { get; set; }
        public int Health { get; set; }
        public int Mana { get; set; }
        public List<string> Abilities { get; set; }
        public string WeaponType { get; set; }
        public string CharacterClass { get; set; }
        public string ArmorType { get; set; }

        public Character()
        {
            Abilities = new List<string>();
        }

        public Character(string name, int level, int health, int mana, List<string> abilities, 
                        string weaponType, string characterClass, string armorType)
        {
            Name = name;
            Level = level;
            Health = health;
            Mana = mana;
            Abilities = abilities ?? new List<string>();
            WeaponType = weaponType;
            CharacterClass = characterClass;
            ArmorType = armorType;
        }

        public object Clone()
        {
            Character clone = (Character)this.MemberwiseClone();
            clone.Abilities = new List<string>(this.Abilities);
            return clone;
        }

        public override string ToString()
        {
            return $"{Name} (Lvl {Level}) - {CharacterClass}";
        }
    }

    // Головна форма програми
    public class MainForm : Form
    {
        private ListBox charactersListBox;
        private Button createButton;
        private Button cloneButton;
        private Button editButton;
        private Button saveJsonButton;
        private Button saveXmlButton;
        private Button loadJsonButton;
        private Button loadXmlButton;
        private Button deleteButton;

        private List<Character> characters;

        public MainForm()
        {
            InitializeComponents();
            characters = new List<Character>();
        }

        private void InitializeComponents()
        {
            this.Text = "Система керування персонажами";
            this.Size = new System.Drawing.Size(600, 400);

            charactersListBox = new ListBox
            {
                Dock = DockStyle.Left,
                Width = 300
            };

            Panel buttonPanel = new Panel
            {
                Dock = DockStyle.Fill
            };

            createButton = new Button
            {
                Text = "Створити персонажа",
                Width = 200,
                Location = new System.Drawing.Point(50, 20)
            };
            createButton.Click += CreateButton_Click;

            cloneButton = new Button
            {
                Text = "Клонувати персонажа",
                Width = 200,
                Location = new System.Drawing.Point(50, 60)
            };
            cloneButton.Click += CloneButton_Click;

            editButton = new Button
            {
                Text = "Редагувати персонажа",
                Width = 200,
                Location = new System.Drawing.Point(50, 100)
            };
            editButton.Click += EditButton_Click;

            deleteButton = new Button
            {
                Text = "Видалити персонажа",
                Width = 200,
                Location = new System.Drawing.Point(50, 140)
            };
            deleteButton.Click += DeleteButton_Click;

            saveJsonButton = new Button
            {
                Text = "Зберегти у JSON",
                Width = 200,
                Location = new System.Drawing.Point(50, 200)
            };
            saveJsonButton.Click += SaveJsonButton_Click;

            saveXmlButton = new Button
            {
                Text = "Зберегти у XML",
                Width = 200,
                Location = new System.Drawing.Point(50, 240)
            };
            saveXmlButton.Click += SaveXmlButton_Click;

            loadJsonButton = new Button
            {
                Text = "Завантажити з JSON",
                Width = 200,
                Location = new System.Drawing.Point(50, 280)
            };
            loadJsonButton.Click += LoadJsonButton_Click;

            loadXmlButton = new Button
            {
                Text = "Завантажити з XML",
                Width = 200,
                Location = new System.Drawing.Point(50, 320)
            };
            loadXmlButton.Click += LoadXmlButton_Click;

            buttonPanel.Controls.Add(createButton);
            buttonPanel.Controls.Add(cloneButton);
            buttonPanel.Controls.Add(editButton);
            buttonPanel.Controls.Add(deleteButton);
            buttonPanel.Controls.Add(saveJsonButton);
            buttonPanel.Controls.Add(saveXmlButton);
            buttonPanel.Controls.Add(loadJsonButton);
            buttonPanel.Controls.Add(loadXmlButton);

            this.Controls.Add(charactersListBox);
            this.Controls.Add(buttonPanel);
        }

        private void CreateButton_Click(object sender, EventArgs e)
        {
            CharacterForm characterForm = new CharacterForm();
            if (characterForm.ShowDialog() == DialogResult.OK)
            {
                characters.Add(characterForm.Character);
                UpdateCharactersList();
            }
        }

        private void CloneButton_Click(object sender, EventArgs e)
        {
            if (charactersListBox.SelectedIndex >= 0)
            {
                Character original = characters[charactersListBox.SelectedIndex];
                Character clone = (Character)original.Clone();
                clone.Name += " (копія)";
                characters.Add(clone);
                UpdateCharactersList();
            }
            else
            {
                MessageBox.Show("Виберіть персонажа для клонування.");
            }
        }

        private void EditButton_Click(object sender, EventArgs e)
        {
            if (charactersListBox.SelectedIndex >= 0)
            {
                Character selectedCharacter = characters[charactersListBox.SelectedIndex];
                CharacterForm characterForm = new CharacterForm(selectedCharacter);
                
                if (characterForm.ShowDialog() == DialogResult.OK)
                {
                    characters[charactersListBox.SelectedIndex] = characterForm.Character;
                    UpdateCharactersList();
                }
            }
            else
            {
                MessageBox.Show("Виберіть персонажа для редагування.");
            }
        }

        private void DeleteButton_Click(object sender, EventArgs e)
        {
            if (charactersListBox.SelectedIndex >= 0)
            {
                characters.RemoveAt(charactersListBox.SelectedIndex);
                UpdateCharactersList();
            }
            else
            {
                MessageBox.Show("Виберіть персонажа для видалення.");
            }
        }

        private void SaveJsonButton_Click(object sender, EventArgs e)
        {
            if (characters.Count == 0)
            {
                MessageBox.Show("Немає персонажів для збереження.");
                return;
            }

            try
            {
                string json = JsonSerializer.Serialize(characters, new JsonSerializerOptions { WriteIndented = true });
                File.WriteAllText("characters.json", json);
                MessageBox.Show("Персонажі успішно збережені у файл characters.json");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Помилка при збереженні у JSON: {ex.Message}");
            }
        }

        private void SaveXmlButton_Click(object sender, EventArgs e)
        {
            if (characters.Count == 0)
            {
                MessageBox.Show("Немає персонажів для збереження.");
                return;
            }

            try
            {
                XmlSerializer serializer = new XmlSerializer(typeof(List<Character>));
                using (FileStream fs = new FileStream("characters.xml", FileMode.Create))
                {
                    serializer.Serialize(fs, characters);
                }
                MessageBox.Show("Персонажі успішно збережені у файл characters.xml");
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Помилка при збереженні у XML: {ex.Message}");
            }
        }

        private void LoadJsonButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (File.Exists("characters.json"))
                {
                    string json = File.ReadAllText("characters.json");
                    characters = JsonSerializer.Deserialize<List<Character>>(json);
                    UpdateCharactersList();
                    MessageBox.Show("Персонажі успішно завантажені з файлу characters.json");
                }
                else
                {
                    MessageBox.Show("Файл characters.json не знайдено.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Помилка при завантаженні з JSON: {ex.Message}");
            }
        }

        private void LoadXmlButton_Click(object sender, EventArgs e)
        {
            try
            {
                if (File.Exists("characters.xml"))
                {
                    XmlSerializer serializer = new XmlSerializer(typeof(List<Character>));
                    using (FileStream fs = new FileStream("characters.xml", FileMode.Open))
                    {
                        characters = (List<Character>)serializer.Deserialize(fs);
                    }
                    UpdateCharactersList();
                    MessageBox.Show("Персонажі успішно завантажені з файлу characters.xml");
                }
                else
                {
                    MessageBox.Show("Файл characters.xml не знайдено.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Помилка при завантаженні з XML: {ex.Message}");
            }
        }

        private void UpdateCharactersList()
        {
            charactersListBox.Items.Clear();
            foreach (var character in characters)
            {
                charactersListBox.Items.Add(character);
            }
        }
    }

    // Форма для створення та редагування персонажа
    public class CharacterForm : Form
    {
        private TextBox nameTextBox;
        private NumericUpDown levelNumeric;
        private NumericUpDown healthNumeric;
        private NumericUpDown manaNumeric;
        private ListBox abilitiesListBox;
        private TextBox newAbilityTextBox;
        private Button addAbilityButton;
        private Button removeAbilityButton;
        private ComboBox weaponTypeComboBox;
        private ComboBox characterClassComboBox;
        private ComboBox armorTypeComboBox;
        private Button saveButton;
        private Button cancelButton;

        public Character Character { get; private set; }

        public CharacterForm(Character character = null)
        {
            InitializeComponents();
            
            if (character != null)
            {
                Character = (Character)character.Clone();
                FillFormWithCharacterData();
            }
            else
            {
                Character = new Character();
            }
        }

        private void InitializeComponents()
        {
            this.Text = "Редагування персонажа";
            this.Size = new System.Drawing.Size(450, 550);
            this.FormBorderStyle = FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.StartPosition = FormStartPosition.CenterParent;

            // Ім'я
            Label nameLabel = new Label { Text = "Ім'я:", Location = new System.Drawing.Point(20, 20) };
            nameTextBox = new TextBox { Location = new System.Drawing.Point(150, 20), Width = 250 };

            // Рівень
            Label levelLabel = new Label { Text = "Рівень:", Location = new System.Drawing.Point(20, 50) };
            levelNumeric = new NumericUpDown { Location = new System.Drawing.Point(150, 50), Width = 100, Minimum = 1, Maximum = 100, Value = 1 };

            // Здоров'я
            Label healthLabel = new Label { Text = "Здоров'я:", Location = new System.Drawing.Point(20, 80) };
            healthNumeric = new NumericUpDown { Location = new System.Drawing.Point(150, 80), Width = 100, Minimum = 1, Maximum = 1000, Value = 100 };

            // Мана
            Label manaLabel = new Label { Text = "Мана:", Location = new System.Drawing.Point(20, 110) };
            manaNumeric = new NumericUpDown { Location = new System.Drawing.Point(150, 110), Width = 100, Minimum = 0, Maximum = 1000, Value = 50 };

            // Здібності
            Label abilitiesLabel = new Label { Text = "Здібності:", Location = new System.Drawing.Point(20, 140) };
            abilitiesListBox = new ListBox { Location = new System.Drawing.Point(150, 140), Width = 250, Height = 100 };

            newAbilityTextBox = new TextBox { Location = new System.Drawing.Point(150, 250), Width = 150 };
            addAbilityButton = new Button { Text = "Додати", Location = new System.Drawing.Point(310, 250), Width = 90 };
            addAbilityButton.Click += AddAbilityButton_Click;

            removeAbilityButton = new Button { Text = "Видалити", Location = new System.Drawing.Point(310, 280), Width = 90 };
            removeAbilityButton.Click += RemoveAbilityButton_Click;

            // Тип зброї
            Label weaponTypeLabel = new Label { Text = "Тип зброї:", Location = new System.Drawing.Point(20, 320) };
            weaponTypeComboBox = new ComboBox
            {
                Location = new System.Drawing.Point(150, 320),
                Width = 250,
                DropDownStyle = ComboBoxStyle.DropDownList
            };
            weaponTypeComboBox.Items.AddRange(new string[] { "Меч", "Лук", "Посох", "Кинджал", "Сокира", "Молот" });

            // Клас персонажа
            Label characterClassLabel = new Label { Text = "Клас персонажа:", Location = new System.Drawing.Point(20, 350) };
            characterClassComboBox = new ComboBox
            {
                Location = new System.Drawing.Point(150, 350),
                Width = 250,
                DropDownStyle = ComboBoxStyle.DropDownList
            };
            characterClassComboBox.Items.AddRange(new string[] { "Воїн", "Маг", "Лучник", "Жрець", "Розбійник", "Паладін" });

            // Тип броні
            Label armorTypeLabel = new Label { Text = "Тип броні:", Location = new System.Drawing.Point(20, 380) };
            armorTypeComboBox = new ComboBox
            {
                Location = new System.Drawing.Point(150, 380),
                Width = 250,
                DropDownStyle = ComboBoxStyle.DropDownList
            };
            armorTypeComboBox.Items.AddRange(new string[] { "Легка", "Середня", "Важка", "Магічна" });

            // Кнопки збереження та відміни
            saveButton = new Button { Text = "Зберегти", Location = new System.Drawing.Point(100, 450), Width = 100 };
            saveButton.Click += SaveButton_Click;

            cancelButton = new Button { Text = "Відміна", Location = new System.Drawing.Point(250, 450), Width = 100 };
            cancelButton.Click += CancelButton_Click;

            // Додавання всіх елементів на форму
            this.Controls.Add(nameLabel);
            this.Controls.Add(nameTextBox);
            this.Controls.Add(levelLabel);
            this.Controls.Add(levelNumeric);
            this.Controls.Add(healthLabel);
            this.Controls.Add(healthNumeric);
            this.Controls.Add(manaLabel);
            this.Controls.Add(manaNumeric);
            this.Controls.Add(abilitiesLabel);
            this.Controls.Add(abilitiesListBox);
            this.Controls.Add(newAbilityTextBox);
            this.Controls.Add(addAbilityButton);
            this.Controls.Add(removeAbilityButton);
            this.Controls.Add(weaponTypeLabel);
            this.Controls.Add(weaponTypeComboBox);
            this.Controls.Add(characterClassLabel);
            this.Controls.Add(characterClassComboBox);
            this.Controls.Add(armorTypeLabel);
            this.Controls.Add(armorTypeComboBox);
            this.Controls.Add(saveButton);
            this.Controls.Add(cancelButton);
        }

        private void FillFormWithCharacterData()
        {
            nameTextBox.Text = Character.Name;
            levelNumeric.Value = Character.Level;
            healthNumeric.Value = Character.Health;
            manaNumeric.Value = Character.Mana;
            
            abilitiesListBox.Items.Clear();
            foreach (var ability in Character.Abilities)
            {
                abilitiesListBox.Items.Add(ability);
            }
            
            if (!string.IsNullOrEmpty(Character.WeaponType) && weaponTypeComboBox.Items.Contains(Character.WeaponType))
                weaponTypeComboBox.SelectedItem = Character.WeaponType;
            else if (weaponTypeComboBox.Items.Count > 0)
                weaponTypeComboBox.SelectedIndex = 0;
                
            if (!string.IsNullOrEmpty(Character.CharacterClass) && characterClassComboBox.Items.Contains(Character.CharacterClass))
                characterClassComboBox.SelectedItem = Character.CharacterClass;
            else if (characterClassComboBox.Items.Count > 0)
                characterClassComboBox.SelectedIndex = 0;
                
            if (!string.IsNullOrEmpty(Character.ArmorType) && armorTypeComboBox.Items.Contains(Character.ArmorType))
                armorTypeComboBox.SelectedItem = Character.ArmorType;
            else if (armorTypeComboBox.Items.Count > 0)
                armorTypeComboBox.SelectedIndex = 0;
        }

        private void AddAbilityButton_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(newAbilityTextBox.Text))
            {
                abilitiesListBox.Items.Add(newAbilityTextBox.Text);
                newAbilityTextBox.Clear();
            }
        }

        private void RemoveAbilityButton_Click(object sender, EventArgs e)
        {
            if (abilitiesListBox.SelectedIndex >= 0)
            {
                abilitiesListBox.Items.RemoveAt(abilitiesListBox.SelectedIndex);
            }
        }

        private void SaveButton_Click(object sender, EventArgs e)
        {
            if (string.IsNullOrWhiteSpace(nameTextBox.Text))
            {
                MessageBox.Show("Введіть ім'я персонажа.");
                return;
            }

            if (weaponTypeComboBox.SelectedIndex < 0 || characterClassComboBox.SelectedIndex < 0 || armorTypeComboBox.SelectedIndex < 0)
            {
                MessageBox.Show("Виберіть тип зброї, клас та тип броні.");
                return;
            }

            Character.Name = nameTextBox.Text;
            Character.Level = (int)levelNumeric.Value;
            Character.Health = (int)healthNumeric.Value;
            Character.Mana = (int)manaNumeric.Value;
            
            Character.Abilities = new List<string>();
            foreach (var item in abilitiesListBox.Items)
            {
                Character.Abilities.Add(item.ToString());
            }
            
            Character.WeaponType = weaponTypeComboBox.SelectedItem.ToString();
            Character.CharacterClass = characterClassComboBox.SelectedItem.ToString();
            Character.ArmorType = armorTypeComboBox.SelectedItem.ToString();

            DialogResult = DialogResult.OK;
            Close();
        }

        private void CancelButton_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }
    }
}
