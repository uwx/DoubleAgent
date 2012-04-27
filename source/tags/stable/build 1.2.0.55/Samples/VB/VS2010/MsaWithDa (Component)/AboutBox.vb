Public NotInheritable Class AboutBox

    Public mDaControl As DoubleAgent.Control.Control

    Private Sub OKButton_Click(sender As System.Object, e As System.EventArgs) Handles OKButton.Click
        Me.Close()
    End Sub

    Private Sub MoreButton_Click(sender As System.Object, e As System.EventArgs) Handles MoreButton.Click
        mDaControl.PropertySheet.Visible = True
        mDaControl.PropertySheet.Page = "Copyright"
    End Sub
End Class
