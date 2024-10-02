public partial class AdminForm : Form
{
    public AdminForm()
    {
        InitializeComponent();
    }

    private void btnAddKey_Click(object sender, EventArgs e)
    {
        // Проверка данных
        if (string.IsNullOrEmpty(textBoxNewKey.Text))
        {
            MessageBox.Show("Пожалуйста, введите новый ключ.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            return;
        }

        try
        {
            AddKeyToDatabase(textBoxNewKey.Text);
            MessageBox.Show("Ключ успешно добавлен.", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
        }
        catch (Exception ex)
        {
            MessageBox.Show($"Ошибка при добавлении ключа: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
        }
    }

    private void AddKeyToDatabase(string key)
    {
        // Здесь должен быть ваш код для записи нового ключа в базу данных
        using (StreamWriter writer = new StreamWriter(@"path\to\database\file.txt", true))
        {
            writer.WriteLine(key);
        }
    }
}
