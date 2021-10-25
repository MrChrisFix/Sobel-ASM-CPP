#pragma once
#include <chrono>
#include <windows.h> 
#include <msclr/marshal_cppstd.h>

#include "CppDLL.h"
#include "BMPManager.h"

namespace ProjektJA {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	/// <summary>
	/// Podsumowanie informacji o MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
		//ASM
		typedef int (*FunkcjaASM_t)(); //drugi nawias na argumenty
		FunkcjaASM_t wykonajASM;
	private: System::Windows::Forms::ComboBox^ comboBox2;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;

		Bitmap^ loadedBitmap;

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: W tym miejscu dodaj kod konstruktora
			//
			this->comboBox1->SelectedIndex = 0;
			this->comboBox2->SelectedIndex = 0;
			
			this->openFileDialog1->Title = "Wybierz obraz";
			this->openFileDialog1->FileName = "";
			this->openFileDialog1->Filter = "Bitmaps|*.bmp";
			loadAsmDLL();
		}

	protected:
		/// <summary>
		/// Wyczyœæ wszystkie u¿ywane zasoby.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}

			if (this->loadedBitmap != nullptr) delete this->loadedBitmap;
		}
		void loadAsmDLL()
		{
			HINSTANCE asmDLL = LoadLibrary(L"AsemblerDLL.dll");

			if (!asmDLL)
			{
				//nie udalo sie zaladwac biblioteki
				this->label2->Text += "ERROR_ASM_LOAD";
			}
			else
			{
				this->wykonajASM = (FunkcjaASM_t)GetProcAddress(asmDLL, "FunkcjaASM");// w string jest nazwa funkcji
				if (!wykonajASM)
					this->label2->Text += "ERROR_ASM_WHERE";
			}

		}


	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
	private: System::Windows::Forms::ProgressBar^ progressBar1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^ wczytajToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ zapiszToolStripMenuItem;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;

	protected:

	private:
		/// <summary>
		/// Wymagana zmienna projektanta.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Metoda wymagana do obs³ugi projektanta — nie nale¿y modyfikowaæ
		/// jej zawartoœci w edytorze kodu.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->progressBar1 = (gcnew System::Windows::Forms::ProgressBar());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->wczytajToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->zapiszToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openFileDialog1 = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog1 = (gcnew System::Windows::Forms::SaveFileDialog());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->comboBox2 = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->menuStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(12, 62);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(652, 418);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 536);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(1336, 23);
			this->progressBar1->TabIndex = 2;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(208, 506);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Start";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MyForm::button1_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->wczytajToolStripMenuItem,
					this->zapiszToolStripMenuItem
			});
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(1360, 24);
			this->menuStrip1->TabIndex = 4;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// wczytajToolStripMenuItem
			// 
			this->wczytajToolStripMenuItem->Name = L"wczytajToolStripMenuItem";
			this->wczytajToolStripMenuItem->Size = System::Drawing::Size(60, 20);
			this->wczytajToolStripMenuItem->Text = L"Wczytaj";
			this->wczytajToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::wczytajToolStripMenuItem_Click);
			// 
			// zapiszToolStripMenuItem
			// 
			this->zapiszToolStripMenuItem->Name = L"zapiszToolStripMenuItem";
			this->zapiszToolStripMenuItem->Size = System::Drawing::Size(52, 20);
			this->zapiszToolStripMenuItem->Text = L"Zapisz";
			this->zapiszToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::zapiszToolStripMenuItem_Click);
			// 
			// openFileDialog1
			// 
			this->openFileDialog1->FileName = L"openFileDialog1";
			this->openFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openFileDialog1_FileOk);
			// 
			// saveFileDialog1
			// 
			this->saveFileDialog1->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveFileDialog1_FileOk);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(299, 511);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(87, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Czas wykonania:";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Asembler", L"C++" });
			this->comboBox1->Location = System::Drawing::Point(13, 507);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 6;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(392, 511);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"0 ms";
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(696, 62);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(652, 418);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 8;
			this->pictureBox2->TabStop = false;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(276, 42);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(124, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Zdjêcie przed przeróbk¹:";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(979, 42);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(107, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Zdjêcie po przeróbce";
			// 
			// comboBox2
			// 
			this->comboBox2->FormattingEnabled = true;
			this->comboBox2->Items->AddRange(gcnew cli::array< System::Object^  >(6) { L"1", L"2", L"4", L"8", L"16", L"32" });
			this->comboBox2->Location = System::Drawing::Point(140, 507);
			this->comboBox2->Name = L"comboBox2";
			this->comboBox2->Size = System::Drawing::Size(56, 21);
			this->comboBox2->TabIndex = 11;
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(50, 491);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(53, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"Biblioteka";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(150, 491);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 13);
			this->label6->TabIndex = 13;
			this->label6->Text = L"W¹tki";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1360, 570);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->comboBox2);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->progressBar1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->Name = L"MyForm";
			this->Text = L"Projekt JA";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		auto start = std::chrono::steady_clock::now();
		if (this->comboBox1->SelectedIndex == 0) //Asembler
		{
			wykonajASM();
		}
		else if (this->comboBox1->SelectedIndex == 1) //C++
		{
			//Otworzyc biblioteke C++ i wykoanc algorytm

			tangensMilion();
		}
		auto end = std::chrono::steady_clock::now();

		//this->progressBar1->Value +=; //Tak sie ustawia progressbar

		std::chrono::duration<double> elapsed_seconds = end - start;

		elapsed_seconds *= 1000;
		if (elapsed_seconds.count() > 0.01)
		{
			auto czas = L"" + elapsed_seconds.count() + " ms";
			this->label2->Text = czas;
		}
	}

	private: System::Void openFileDialog1_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
	{
		//If bitmap is selected

		//TODO: Delete old edited bitmap

		this->loadedBitmap = gcnew Bitmap(openFileDialog1->FileName);

		if(loadedBitmap != nullptr)
			pictureBox1->Image = loadedBitmap;

		

		BMPManager klasa(msclr::interop::marshal_as<std::string>(openFileDialog1->FileName));

		Bitmap^ gray = klasa.getBitmap();

		if(gray!=nullptr) this->pictureBox2->Image = gray;

		//Bitmap^ test = klasa.createBitmap(klasa.getPixelArray2D());

		//if (test != nullptr) this->pictureBox2->Image = test;
		
		//Bitmap^ sobelCpp = klasa.createBitmap(Sobel(klasa.getPixelArray2D(), 1, 1, klasa.getHeight(), klasa.getHeight()));

		//if (sobelCpp != nullptr) this->pictureBox2->Image = sobelCpp;
	}


	private: System::Void saveFileDialog1_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e)
	{

	}

	private: System::Void wczytajToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		this->openFileDialog1->ShowDialog();
	}

	private: System::Void zapiszToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		//this->saveFileDialog1->ShowDialog(); //TODO
	}

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
	{
	}
};
}
