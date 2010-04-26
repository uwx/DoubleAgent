#pragma once

namespace DoubleAgent {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for ControlTestForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class ControlTestForm : public System::Windows::Forms::Form
	{
	public:
		ControlTestForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~ControlTestForm()
		{
			if (components)
			{
				delete components;
			}
		}

//	private: AxDoubleAgentCtl::AxDaControl^  OldDaControl;
	private: DoubleAgent::Control::AxControl^  NewDaControl;
	private: System::Windows::Forms::Button^  button1;
	protected: 

	protected: 

	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ControlTestForm::typeid));
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->NewDaControl = (gcnew DoubleAgent::Control::AxControl());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewDaControl))->BeginInit();
			this->SuspendLayout();
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(25, 33);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(95, 30);
			this->button1->TabIndex = 1;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &ControlTestForm::button1_Click);
			// 
			// NewDaControl
			// 
			this->NewDaControl->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom) 
				| System::Windows::Forms::AnchorStyles::Left) 
				| System::Windows::Forms::AnchorStyles::Right));
			this->NewDaControl->BorderColor = System::Drawing::SystemColors::WindowText;
			this->NewDaControl->BorderStyle = 1;
			this->NewDaControl->BorderVisible = true;
			this->NewDaControl->BorderWidth = 1;
			this->NewDaControl->Enabled = true;
			this->NewDaControl->Location = System::Drawing::Point(140, 12);
			this->NewDaControl->Name = L"NewDaControl";
			this->NewDaControl->OcxState = (cli::safe_cast<System::Windows::Forms::AxHost::State^  >(resources->GetObject(L"NewDaControl.OcxState")));
			this->NewDaControl->RaiseRequestErrors = true;
			this->NewDaControl->Size = System::Drawing::Size(160, 157);
			this->NewDaControl->TabIndex = 2;
			// 
			// ControlTestForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(312, 297);
			this->Controls->Add(this->NewDaControl);
			this->Controls->Add(this->button1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"ControlTestForm";
			this->Text = L"Control Test";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->NewDaControl))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
		private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		{
			DoubleAgent::Control::AxControl^  lNewDaControl = NewDaControl;
			DoubleAgent::Control::IControl^  lControl = NewDaControl->mControl;
			DoubleAgent::Control::ICharacters^  lCharacters = NewDaControl->Characters;
			DoubleAgent::Control::ICharacterFiles^  lCharacterFiles = NewDaControl->CharacterFiles;
		}
	};
}

