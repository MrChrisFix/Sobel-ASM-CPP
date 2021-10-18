#pragma once
#include <chrono>
#include <windows.h> 

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
			///////////////////////
			//�adowanie bibliotek//
			///////////////////////
			//ASM
			typedef int (*FunkcjaASM_t)(); //drugi nawias na argumenty
			FunkcjaASM_t wykonajASM;

			typedef int (*FunkcjaCPP_t)(); //drugi nawias na argumenty
			FunkcjaCPP_t wykonajCpp;


			

	public:
		MyForm(void)
		{
			InitializeComponent();
			//
			//TODO: W tym miejscu dodaj kod konstruktora
			//
			this->comboBox1->SelectedIndex = 0;

			loadDLLs();
		}

	protected:
		/// <summary>
		/// Wyczy�� wszystkie u�ywane zasoby.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
		void loadDLLs()
		{
			HINSTANCE asmDLL = LoadLibrary(L"AsemblerDLL.dll");
			FunkcjaASM_t wykonajASM = (FunkcjaASM_t)GetProcAddress(asmDLL, "FunkcjaASM"); // w string jest nazwa funkcji

			//CPP
			typedef int (*FunkcjaCPP_t)(); //drugi nawias na argumenty
			HINSTANCE CppDLL = LoadLibrary(L"CppDLL.dll");
			if (!CppDLL)
			{
				//nie udalo sie zaladwac biblioteki
				this->label2->Text = "ERROR_LOAD";
			}
			else
			{
				FunkcjaCPP_t wykonajCpp = (FunkcjaCPP_t)GetProcAddress(CppDLL, "tangensMilion");
				if (!wykonajCpp)
					this->label2->Text = "ERROR_WHERE";
			}

		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;

	private: System::Windows::Forms::ProgressBar^ progressBar1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::MenuStrip^ menuStrip1;

	private: System::Windows::Forms::OpenFileDialog^ openFileDialog1;
	private: System::Windows::Forms::SaveFileDialog^ saveFileDialog1;
	private: System::Windows::Forms::ToolStripMenuItem^ wczytajToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ zapiszToolStripMenuItem;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::ComboBox^ comboBox1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::PictureBox^ pictureBox2;
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
		/// Metoda wymagana do obs�ugi projektanta � nie nale�y modyfikowa�
		/// jej zawarto�ci w edytorze kodu.
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
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &MyForm::pictureBox1_Click);
			// 
			// progressBar1
			// 
			this->progressBar1->Location = System::Drawing::Point(12, 519);
			this->progressBar1->Name = L"progressBar1";
			this->progressBar1->Size = System::Drawing::Size(1336, 23);
			this->progressBar1->TabIndex = 2;
			this->progressBar1->Click += gcnew System::EventHandler(this, &MyForm::progressBar1_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(160, 490);
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
			this->menuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MyForm::menuStrip1_ItemClicked);
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
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(251, 495);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(87, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Czas wykonania:";
			this->label1->Click += gcnew System::EventHandler(this, &MyForm::label1_Click);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Asembler", L"C++" });
			this->comboBox1->Location = System::Drawing::Point(11, 490);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 6;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MyForm::comboBox1_SelectedIndexChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(344, 495);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(29, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"0 ms";
			this->label2->Click += gcnew System::EventHandler(this, &MyForm::label2_Click);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Location = System::Drawing::Point(696, 62);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(652, 418);
			this->pictureBox2->TabIndex = 8;
			this->pictureBox2->TabStop = false;
			this->pictureBox2->Click += gcnew System::EventHandler(this, &MyForm::pictureBox2_Click);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(276, 42);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(124, 13);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Zdj�cie przed przer�bk�:";
			this->label3->Click += gcnew System::EventHandler(this, &MyForm::label3_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(979, 42);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(107, 13);
			this->label4->TabIndex = 10;
			this->label4->Text = L"Zdj�cie po przer�bce";
			this->label4->Click += gcnew System::EventHandler(this, &MyForm::label4_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1360, 554);
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
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void pictureBox2_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
		auto start = std::chrono::steady_clock::now();
		if (this->comboBox1->SelectedIndex == 0) //Asembler
		{
			
			wykonajASM();

			/*for (int i = 0; i < 1000000; i++)
				auto tangens = tan(i);*/
		}
		else if (this->comboBox1->SelectedIndex == 1) //C++
		{
			//Otworzyc biblioteke C++ i wykoanc algorytm

			wykonajCpp();
		}
		auto end = std::chrono::steady_clock::now();

		std::chrono::duration<double> elapsed_seconds = end - start;

		elapsed_seconds *= 1000;
		if (elapsed_seconds.count() > 0.01)
		{
			auto czas = L"" + elapsed_seconds.count() + " ms";
			this->label2->Text = czas;
		}
	}
	private: System::Void progressBar1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void openFileDialog1_FileOk(System::Object^ sender, System::ComponentModel::CancelEventArgs^ e) 
	{
	}
	private: System::Void menuStrip1_ItemClicked(System::Object^ sender, System::Windows::Forms::ToolStripItemClickedEventArgs^ e) 
	{
	}
	private: System::Void wczytajObrazToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void wczytajToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void zapiszToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void label3_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
	private: System::Void label4_Click(System::Object^ sender, System::EventArgs^ e) 
	{
	}
};
}